#ifndef IMGMAP_IMGMAP_H__
#define IMGMAP_IMGMAP_H__
#include <stddef.h>

enum IMGMAP_ERRORS {
  IMGMAP_OK           =  0,
  IMGMAP_EPARSE       = -1,
  IMGMAP_ESTATFILE    = -2,
  IMGMAP_EINVALIDFILE = -3,
  IMGMAP_EMMAP        = -4,
  IMGMAP_EINVALIDMODE = -5,
  IMGMAP_EOPENFILE    = -6,
  IMGMAP_ETRUNCATE    = -7
};

enum IMGMAP_MODES {
  IMGMAP_READPRIVATE = 0,
  IMGMAP_RWPRIVATE   = 1,
  IMGMAP_READSHARED  = 2,
  IMGMAP_RWSHARED    = 3
};

enum IMGMAP_TYPES {
  IMGMAP_INVALID    = -1,
  // Raw values ordered: [nl][sy][sx][nc]
  IMGMAP_RAW1BYTE   = 0, // Values in one byte format
  IMGMAP_RAW2BYTEBE = 1, // Values in two bytes big endian
  IMGMAP_TEXTPBM    = 2, // Text format used by PPM/PGM/PBM
  IMGMAP_RAW1BPP    = 3, // Raw bitmap used by PPM
  IMGMAP_RAW2BPP    = 4,
  IMGMAP_RAW4BPP    = 5
};

enum IMGMAP_PALETTES {
  IMGMAP_BW   = 0,
  IMGMAP_GREY = 1,
  IMGMAP_RGB  = 2,
  IMGMAP_RGBA = 3,
  IMGMAP_BGR  = 4
};

typedef struct {
  void *map; // Start address of the map
  size_t map_size; // Size of the map
  void *data; // Start of the raw image data
  void *end; // Size of the raw data segment
  int mode; // Mode used for opening/creating the file
  int data_type; // Type of the raw data
  int sx, sy; // Size in x and y
  int nc, nl; // Number of channels and layers
  int dx; // Delta value between two lines
  int max_val; // Maximum value of a color
  int palette_type;
  void *palette; // Pointer to the color palette
} IMGMAP_FILE;

int imgmap_close(IMGMAP_FILE *fmap);
int imgmap_open(IMGMAP_FILE *fmap, const char *name, int mode);
int imgmap_create(IMGMAP_FILE *fmap, const char *name, int mode);

#endif

