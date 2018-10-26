#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "imgmap.h"

int main(int argc, char **argv) {
  IMGMAP_FILE fmap;
  int ret = imgmap_createImg(&fmap, "test.pgm", IMGMAP_RWSHARED,
      IMGMAPFILE_PGM, 1, 64, 64, 255);
  float f[64][64];
  int i,j;
  for(i=0; i<64; i++)
    for(j=0; j<64; j++)
      f[i][j] = ((float) (64-i)*j)/4096.0;
  imgmap_pushFloatValue(&fmap, (float *) f);
  imgmap_sync(&fmap, IMGMAP_SYNC);
  imgmap_close(&fmap);
  return ret;
}
