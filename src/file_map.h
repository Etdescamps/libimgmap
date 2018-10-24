#ifndef IMGMAP_FILE_MAP_H__
#define IMGMAP_FILE_MAP_H__
#include "imgmap.h"

int imgmap_loadMapId(IMGMAP_FILE *fmap, int file_id, int mode);
int imgmap_loadMapFile(IMGMAP_FILE *fmap, const char *name, int mode);

int imgmap_createFile(IMGMAP_FILE *fmap, size_t size,
    const char *name, int mode);
int imgmap_pgm_readHeader(IMGMAP_FILE *fmap, int type,
    const char **p, const char *end);

#endif //IMGMAP_FILE_MAP_H__

