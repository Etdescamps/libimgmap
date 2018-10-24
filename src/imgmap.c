#include "imgmap.h"
#include "file_map.h"

int imgmap_close(IMGMAP_FILE *fmap) {
  if(fmap->map)
    return munmap(fmap->map, fmap->map_size);
  fmap->map = NULL;
  return IMGMAP_OK;
}

int imgmap_open(IMGMAP_FILE *fmap, const char *name, int mode) {
  return IMGMAP_OK;
}

int imgmap_create(IMGMAP_FILE *fmap, const char *name, int mode) {
  return IMGMAP_OK;
}

