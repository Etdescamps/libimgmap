/*
 * Copyright (c) 2018 Etienne Descamps
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "imgmap.h"
#include "file_map.h"
#include "file_parse.h"
#include "file_types.h"
#include "data_conv.h"

IMGMAP_FILE *imgmap_allocate() {
  IMGMAP_FILE *fmap = malloc(sizeof(IMGMAP_FILE));
  if(!fmap)
    return NULL;
  fmap->map = NULL;
  fmap->file_id = -1;
  return fmap;
}

int imgmap_free(IMGMAP_FILE *fmap) {
  int ret = imgmap_close(fmap);
  free(fmap);
  return ret;
}

int imgmap_syncPart(IMGMAP_FILE *fmap, int flags, void *begin, void *end) {
  if(!fmap || begin >= end)
    return -1;
  if(begin < fmap->map)
    return -1;
  if(end >= fmap->end)
    return -1;
  return imgmap_memSync(begin, end, flags);
}

int imgmap_sync(IMGMAP_FILE *fmap, int flags) {
  return imgmap_memSync(fmap->map, fmap->end, flags);
}

int imgmap_close(IMGMAP_FILE *fmap) {
  if(fmap->map) {
    return imgmap_unmap(fmap);
  } 
  if(fmap->file_id > 2)
    return close(fmap->file_id);
  return IMGMAP_OK;
}

int imgmap_createBuffer(IMGMAP_FILE *fmap, int data_type,
    int nl, int nc, int sx, int sy) {
  size_t dx = nc*sx;
  size_t numValues = nl*sy*dx;
  size_t size;
  switch(data_type & IMGMAP_TYPEMASK) {
    case IMGMAP_RAW1BYTE:
      size = numValues;
      break;
    case IMGMAP_RAW2BYTELE:
    case IMGMAP_RAW2BYTEBE:
    case IMGMAP_FLOAT16BE:
    case IMGMAP_FLOAT16LE:
      size = 2*numValues;
      break;
    case IMGMAP_FLOAT32LE:
    case IMGMAP_FLOAT32BE:
      size = 4*numValues;
      break;
    case IMGMAP_FLOAT64LE:
    case IMGMAP_FLOAT64BE:
      size = 8*numValues;
      break;
    case IMGMAP_RAW1BPP:
      dx = ((dx-1)/8+1)*8;
      size = nl*sy*dx;
    break;
    default:
      return IMGMAP_ENOTSUPPORTED;
  }
  int ret = imgmap_mapBuffer(fmap, size, IMGMAP_ANONYMOUS, 0);
  if(ret < 0)
    return IMGMAP_EMMAP;
  fmap->dx = dx;
  fmap->sx = sx;
  fmap->sy = sy;
  fmap->nc = nc;
  fmap->nl = nl;
  fmap->data = fmap->map;
  return IMGMAP_OK;
}

static int _openFile(IMGMAP_FILE *fmap) {
  const char *p = fmap->map;
  int file_type = imgmap_getTypeKeyword(p);
  return imgmap_fileTypeInit(fmap, file_type);
}

int imgmap_open(IMGMAP_FILE *fmap, const char *name, int mode) {
  int rep = imgmap_loadMapFile(fmap, name, mode);
  if(rep < IMGMAP_OK)
    return rep;
  return _openFile(fmap);
}

int imgmap_getDimensions(const IMGMAP_FILE *fmap, int *sx, int *sy,
    int *nc, int *nl) {
  if(!fmap)
    return -1;
  if(sx) {
    *sx = fmap->sx;
    if(*sx <= 0)
      return -2;
  }
  if(sy) {
    *sy = fmap->sy;
    if(*sy <= 0)
      return -2;
  }
  if(nc) {
    *nc = fmap->nc;
    if(*nc <= 0)
      return -2;
  }
  if(nl) {
    *nl = fmap->nl;
    if(*nl <= 0)
      return -2;
  }
  return 0;
}

int imgmap_createImg(IMGMAP_FILE *fmap, const char *name, int sx, int sy) {
  return imgmap_createImgSpec(fmap, name, IMGMAPFILE_UKN, sx, sy, -1, -1, -1);
}

int imgmap_createImgSpec(IMGMAP_FILE *fmap, const char *name, int file_type,
    int sx, int sy, int nc, int nl, int max_val) {
  int mode = IMGMAP_RWSHARED; // The only suitable mode for creating file
  fmap->sx = sx;
  fmap->sy = sy;
  fmap->nc = nc;
  fmap->nl = nl;
  fmap->max_val = max_val;
  if(file_type == IMGMAPFILE_UKN) // Use extension for determining type
    file_type = imgmap_getTypeFilename(name);
  switch(file_type) {
    case IMGMAPFILE_PBM:
      return imgmap_createPBM(fmap, name, mode, 4);
    case IMGMAPFILE_PGM:
      return imgmap_createPBM(fmap, name, mode, 5);
    case IMGMAPFILE_PPM:
      return imgmap_createPBM(fmap, name, mode, 6);
    default:
      return IMGMAP_ENOTSUPPORTED;
  }
}

int imgmap_getFloatValues(IMGMAP_FILE *fmap, float *dest) {
  if(!fmap->data)
    return IMGMAP_EINVALIDFILE;
  size_t numPixels = fmap->nc*fmap->sx*fmap->sy*fmap->nl, n;
  const char *data = fmap->data;
  switch(fmap->data_type & IMGMAP_TYPEMASK) {
    case IMGMAP_RAW1BYTE:
      imgmap_convByteToFloat(dest, (const unsigned char *)data,
          numPixels, fmap->max_val);
      return IMGMAP_OK;
    case IMGMAP_RAW2BYTEBE:
      if(is_short16LE())
        imgmap_convInvShortToFloat(dest, (const unsigned short*) data,
            numPixels, fmap->max_val);
      else
        imgmap_convShortToFloat(dest, (const unsigned short*) data,
            numPixels, fmap->max_val);
      return IMGMAP_OK;
    case IMGMAP_RAW2BYTELE:
      if(is_short16LE())
        imgmap_convShortToFloat(dest, (const unsigned short*) data,
            numPixels, fmap->max_val);
      else
        imgmap_convInvShortToFloat(dest, (const unsigned short*) data,
            numPixels, fmap->max_val);
      return IMGMAP_OK;
    case IMGMAP_RAW1BPP:
      imgmap_convPBMToFloat(dest, data, fmap->sy, fmap->sx);
      return IMGMAP_OK;
    case IMGMAP_TEXTPBM:
      n = imgmap_parse_readNTextFloat(dest, numPixels, fmap->max_val,
          &data, (const char*) fmap->end);
      if(n<numPixels)
        memset(&dest[n], 0, (numPixels-n)*sizeof(float));
      return IMGMAP_OK;
    default:
      return IMGMAP_EINVALIDFILE;
  }
}

int imgmap_pushFloatValues(IMGMAP_FILE *fmap, const float *src) {
  if(!fmap->data)
    return IMGMAP_EINVALIDFILE;
  size_t numPixels = fmap->nc*fmap->sx*fmap->sy*fmap->nl;
  unsigned char *data = fmap->data;
  switch(fmap->data_type & IMGMAP_TYPEMASK) {
    case IMGMAP_RAW1BYTE:
      imgmap_convFloatToByte(data, src, numPixels, fmap->max_val);
      return IMGMAP_OK;
    case IMGMAP_RAW2BYTEBE:
      if(is_short16LE())
        imgmap_convFloatToInvShort((unsigned short*) data, src,
            numPixels, fmap->max_val);
      else
        imgmap_convFloatToShort((unsigned short*) data, src,
            numPixels, fmap->max_val);
      return IMGMAP_OK;
    case IMGMAP_RAW2BYTELE:
      if(is_short16LE())
        imgmap_convFloatToShort((unsigned short*) data, src,
            numPixels, fmap->max_val);
      else
        imgmap_convFloatToInvShort((unsigned short*) data, src,
            numPixels, fmap->max_val);
      return IMGMAP_OK;
    case IMGMAP_RAW1BPP:
      // TODO
    case IMGMAP_TEXTPBM: // This one in not applicable
    default:
      return IMGMAP_EINVALIDFILE;
  }
}

void *imgmap_getRaw(IMGMAP_FILE *fmap, int *data_type, int *palette_type,
    void **palette) {
  if(data_type)
    *data_type = fmap->data_type;
  if(palette_type)
    *palette_type = fmap->palette_type;
  if(palette)
    *palette = fmap->palette;
  return fmap->data;
}

