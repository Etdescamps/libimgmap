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
#include "file_types.h"
#include "imgmap.h"

int imgmap_getTypeKeyword(const char *p) {
  switch(*p) {
    case 'P':
      if(p[1] >= '1' && p[1] <= '6')
        return IMGMAPFILE_PBM;
      break;
    case 10:
      if(p[1] <= 5)
        return IMGMAPFILE_PCX;
      break;
    case 'B':
      if(p[1] == 'M')
        return IMGMAPFILE_BMP;
      break;
    case 'I':
      if(p[1] == 'I')
        return IMGMAPFILE_TIFFLE;
      break;
    case 'M':
      if(p[1] == 'M')
        return IMGMAPFILE_TIFFBE;
      break;
  }
  return IMGMAPFILE_UKN;
}

int imgmap_fileTypeInit(IMGMAP_FILE *fmap, int file_type) {
  switch(file_type) {
    case IMGMAPFILE_PBM:
      return imgmap_pgm_readHeader(fmap);
    case IMGMAPFILE_BMP:
    case IMGMAPFILE_PCX:
    case IMGMAPFILE_TIFFLE:
    case IMGMAPFILE_TIFFBE:
      return IMGMAP_EUNIMPLEMENTED;
    default:
      return IMGMAP_EINVALIDFILE;
  }
}


