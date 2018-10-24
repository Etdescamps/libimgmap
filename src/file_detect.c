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


