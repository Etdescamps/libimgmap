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

#include <stdio.h>
#include <string.h>
#include "file_parse.h"
#include "file_map.h"

#define TEXT_BUFFER_SIZE__ 256

static int _pbmNextInt(const char **p, const char *end) {
  int ret = IMGMAP_OK;
  for(;;) {
    ret = imgmap_parse_space(p, end);
    if(ret < IMGMAP_OK)
      return ret;
    // Comment section
    if(**p == '#') {
      ret = imgmap_parse_moveNextLine(p, end);
      if(ret < IMGMAP_OK)
        return ret;
    }
    else // Shall be another the first character of an integer
      return imgmap_parse_readPosInt(p, end);
  }
}

// Configure fmap in order to match the file structure
static size_t _pbm_configure(IMGMAP_FILE *fmap, int type) {
  fmap->palette = NULL; // No palette support on this type
  fmap->nl = 1; // Supports only one layer

  // Determine number of channels and palette_type
  if(type == 3 || type == 6) {
    // RGB images
    fmap->nc = 3;
    fmap->palette_type = IMGMAP_RGB;
  }
  else {
    // B&W and greyscale images
    fmap->nc = 1;
    if(type == 1 || type == 4) // B&W
      fmap->palette_type = IMGMAP_BW;
    else // Grey
      fmap->palette_type = IMGMAP_GREY;
  }

  // Determine data_type and dx
  if(type <= 3) {
    fmap->data_type = IMGMAP_TEXTPBM;
    fmap->dx = -1; // Not applicable: text file
  }
  else if(type == 4) {
    fmap->data_type = IMGMAP_RAW1BPP;
    fmap->dx = ((fmap->sx-1)/8+1);
  }
  else  { // (type == 5 || type == 6)
    if(fmap->max_val < 256) {
      fmap->data_type = IMGMAP_RAW1BYTE;
      fmap->dx = fmap->sx*fmap->nc;
    }
    else { // Extension up to 65535 using two bytes
      fmap->data_type = IMGMAP_RAW2BYTEBE;
      fmap->dx = 2*fmap->sx*fmap->nc;
    }
  }
  return fmap->dx*fmap->sy;
}

int imgmap_pgm_readHeader(IMGMAP_FILE *fmap) {
  const char *p = (const char *) fmap->map;
  const char *end = (const char *) fmap->end;
  if(p[0] != 'P' || p[1] < '1' || p[1] > '6')
    return IMGMAP_EINVALIDFILE;
  int type = p[1] - '0';
  p += 2;
  // Read sx and sy
  if((fmap->sx = _pbmNextInt(&p,end)) < 0)
    return IMGMAP_EPARSE;
  if((fmap->sy = _pbmNextInt(&p,end)) < 0)
    return IMGMAP_EPARSE;
  if(type != 1 && type !=4)  { // read max_value field of grey/RGB image
    if((fmap->max_val = _pbmNextInt(&p,end)) < 0)
      return IMGMAP_EPARSE;
  }
  else // B&W image do not have max_value field (max_val == 1)
    fmap->max_val = 1;
  ++p; // Ignore one space character
  fmap->data = (void*) p;
  if(_pbm_configure(fmap, type) == 0)
    return IMGMAP_EINVALIDFILE;
  return IMGMAP_OK;
}

int imgmap_createPBM(IMGMAP_FILE *fmap, const char *name, int mode,
    int type, int max_val) {
  char buffer[TEXT_BUFFER_SIZE__];
  int k = snprintf(buffer, TEXT_BUFFER_SIZE__,
                   "P%d\n#fastpbm\n%d %d\n",
                   type, fmap->sx, fmap->sy);
  fmap->max_val = max_val;
  if(type != 4)
    k += snprintf(&buffer[k], TEXT_BUFFER_SIZE__-k,
                   "%d\n", max_val);
  size_t size = k + _pbm_configure(fmap, type);
  int ret = imgmap_createFile(fmap, size, name, mode);
  if(ret<0)
    return ret;
  memcpy(fmap->map, buffer, k);
  fmap->data = &((char*) fmap->map)[k];
  return imgmap_memSync(fmap->map, fmap->data, IMGMAP_ASYNC);
}

