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

#include <sys/mman.h>
#include <string.h>
#include "imgmap.h"
#include "file_map.h"
#include "file_parse.h"
#include "file_types.h"
#include "data_conv.h"

#define TEXT_BUFFER_SIZE__ 256

int imgmap_close(IMGMAP_FILE *fmap) {
  if(fmap->map)
    return munmap(fmap->map, fmap->map_size);
  fmap->map = NULL;
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

int imgmap_createImg(IMGMAP_FILE *fmap, const char *name, int mode,
    int file_type, int nc, int sx, int sy, int max_val) {
  int filetype;
  fmap->sx = sx;
  fmap->sy = sy;
  fmap->nc = nc;
  switch(file_type) {
    case IMGMAPFILE_PBM:
      if(nc == 3)
        filetype = 6;
      else if(max_val == 1)
        filetype = 4;
      else
        filetype = 5;
      return imgmap_createPBM(fmap, name, mode, filetype, max_val);
  }
  return IMGMAP_ENOTSUPPORTED;
}

int imgmap_getFloatValue(IMGMAP_FILE *fmap, float *dest) {
  if(!fmap->data)
    return IMGMAP_EINVALIDFILE;
  size_t numPixels = fmap->nc*fmap->sx*fmap->sy*fmap->nl, n;
  const char *data = fmap->data;
  switch(fmap->data_type) {
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

