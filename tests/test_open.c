#include <stdio.h>
#include "imgmap.h"

int main(int argc, char **argv) {
  if(argc<2)
    return 1;
  IMGMAP_FILE fmap;
  int ret;
  if((ret = imgmap_open(&fmap, argv[1], IMGMAP_READPRIVATE))<0) {
    fprintf(stderr, "Error %d while openind %s\n", ret, argv[1]);
    return ret;
  }
  printf("%s: %d %d %d %d %d\n", argv[1], fmap.data_type, fmap.sx, fmap.sy,
      fmap.max_val, fmap.nc);
  return imgmap_close(&fmap);
}

