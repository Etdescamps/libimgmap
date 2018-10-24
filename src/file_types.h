#ifndef IMGMAP_FILE_TYPES_H__
#define IMGMAP_FILE_TYPES_H__

#include "imgmap.h"
int imgmap_getTypeKeyword(const char *p);

int imgmap_fileTypeInit(IMGMAP_FILE *fmap, int file_type);
int imgmap_pgm_readHeader(IMGMAP_FILE *fmap);

#endif

