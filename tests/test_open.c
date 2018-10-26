#include <stdio.h>
#include <stdlib.h>
#include "imgmap.h"

int main(int argc, char **argv) {
  if(argc<2)
    return 1;
  IMGMAP_FILE fmap;
  int ret;
  if((ret = imgmap_open(&fmap, argv[1], IMGMAP_READPRIVATE))<0) {
    fprintf(stderr, "Error %d while opening %s\n", ret, argv[1]);
    return ret;
  }
  int sx, sy, nc, nl;
  imgmap_getDimensions(&fmap, &sx, &sy, &nc, &nl);
  size_t i, nPix = sx*sy*nc*nl;
  float *f = calloc(sx*sy*nc*nl, sizeof(float));
  imgmap_getFloatValues(&fmap, f);
  for(i = 0; i < nPix; i++) {
    printf("%.1f ", f[i]);
    if(i%20 == 0)
      printf("\n");
  }
  free(f);
  printf("\n%s: %d %d %d %d %d\n", argv[1], fmap.data_type, fmap.sx, fmap.sy,
      fmap.max_val, fmap.nc);
  return imgmap_close(&fmap);
}

