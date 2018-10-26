#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "imgmap.h"

int main(int argc, char **argv) {
  IMGMAP_FILE fmap;
  int ret = imgmap_createImg(&fmap, "test.pgm", IMGMAP_RWSHARED,
      IMGMAPFILE_PGM, 1, 64, 64, 255);
  imgmap_close(&fmap);
  return ret;
}
