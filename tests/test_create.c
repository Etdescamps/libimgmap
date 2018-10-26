#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "imgmap.h"

int test_createImg(const char *name, const float *values,
    int sx, int sy, int max_val) {
  IMGMAP_FILE fmap;
  int ret = imgmap_createImgSpec(&fmap, name,
      IMGMAPFILE_UKN, sx, sy, -1, -1, max_val);
  if(ret<0)
    return ret;
  ret = imgmap_pushFloatValues(&fmap, values);
  if(ret<0)
    return ret;
  ret = imgmap_sync(&fmap, IMGMAP_ASYNC);
  if(ret<0)
    return ret;
  return imgmap_close(&fmap);
}

int main(int argc, char **argv) {
  float f[64][64];
  float g[64][64][3];
  int i,j;
  for(i=0; i<64; i++)
    for(j=0; j<64; j++) {
      f[i][j] = ((float) (64-i)*j)/4096.0;
      g[i][j][0] = ((float) (64-i)*j)/4096.0;
      g[i][j][1] = ((float) i*j)/4096.0;
      g[i][j][2] = ((float) i*(64-j))/4096.0;
    }
  int ret = test_createImg("test.pgm", (float*) f, 64, 64, 255);
  if(ret<0)
    return ret;
  ret = test_createImg("test2.pgm", (float*) f, 64, 64, 65535);
  if(ret<0)
    return ret;
  ret = test_createImg("test.ppm", (float*) g, 64, 64, 255);
  if(ret<0)
    return ret;
  return test_createImg("test2.ppm", (float*) g, 64, 64, 65535);
}
