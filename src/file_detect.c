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
#include <ctype.h>
#include "data_conv.h"
#include "file_types.h"
#include "imgmap.h"

int imgmap_getTypeKeyword(const char *p) {
  switch(*p) {
    case 'P':
      switch(p[1]) {
        case '1':
        case '4':
          return IMGMAPFILE_PBM;
        case '2':
        case '5':
          return IMGMAPFILE_PGM;
        case '3':
        case '6':
          return IMGMAPFILE_PPM;
      }
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

int imgmap_getTypeFilename(const char *filename) {
  const char *pnt_ext = strrchr(filename, '.');
  pnt_ext++; // move after the point
  unsigned char ext[4] = {0}; // For file with extention of size 4
  int i;
  for(i = 0; i<4; i++) {
    if(pnt_ext == 0)
      break;
    ext[i] = toupper((unsigned char) pnt_ext[i]);
  }
  switch(ext[0]) {
    case 'B':
      if(ext[1]=='M' && ext[2] == 'P')
        return IMGMAPFILE_BMP;
      return IMGMAPFILE_UKN;
    case 'P':
      if(ext[1]=='C' && ext[2] == 'X')
        return IMGMAPFILE_PCX;
      else if(ext[2] == 'M') {
        if(ext[1] == 'B')
          return IMGMAPFILE_PBM;
        else if(ext[1] == 'G')
          return IMGMAPFILE_PGM;
        else if(ext[1] == 'P')
          return IMGMAPFILE_PPM;
      }
      return IMGMAPFILE_UKN;
    case 'T':
      if(ext[1]=='I' && ext[2]=='F') {
        if(is_short16LE())
          return IMGMAPFILE_TIFFLE;
        else
          return IMGMAPFILE_TIFFBE;
      }
    default:
      return IMGMAPFILE_UKN;
  }
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


