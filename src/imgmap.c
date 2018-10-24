#include <sys/mman.h>
#include "imgmap.h"
#include "file_map.h"
#include "file_types.h"

int imgmap_close(IMGMAP_FILE *fmap) {
  if(fmap->map)
    return munmap(fmap->map, fmap->map_size);
  fmap->map = NULL;
  return IMGMAP_OK;
}

static int _openFile(IMGMAP_FILE *fmap) {
  const char *p = fmap->map;
  int file_type = imgmap_getTypeKeyword(p);
  return imgmap_fileTypeInit(fmap, file_type);
}

int imgmap_open(IMGMAP_FILE *fmap, const char *name, int mode) {
  int rep = imgmap_loadMapFile(fmap, name, mode);
  if(rep < IMGMAP_OK)
    return rep;
  return _openFile(fmap);
}

int imgmap_createPGM(IMGMAP_FILE *fmap, const char *name, int mode,
    int type, int sx, int sy, int max_value) {
  return IMGMAP_EUNIMPLEMENTED;
}

