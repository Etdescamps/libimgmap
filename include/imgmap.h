/*
 * Copyright (c) 2018 Etienne Descamps
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

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
  IMGMAP_ETRUNCATE    = -7,
  IMGMAP_EUNIMPLEMENTED = -8,
  IMGMAP_ENOTSUPPORTED  = -9
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
  IMGMAP_RAW2BYTELE = 1, // Values in two bytes little endian
  IMGMAP_RAW2BYTEBE = 2, // Values in two bytes big endian
  IMGMAP_TEXTPBM    = 3, // Text format used by PPM/PGM/PBM
  IMGMAP_RAW1BPP    = 4, // Raw bitmap used by PPM
  IMGMAP_RAW2BPP    = 5,
  IMGMAP_RAW4BPP    = 3
};

enum IMGMAP_PALETTES {
  IMGMAP_BW   = 0,
  IMGMAP_GREY = 1,
  IMGMAP_RGB  = 2,
  IMGMAP_RGBA = 3,
  IMGMAP_BGR  = 4,
  IMGMAP_CMYK = 5,
  IMGMAP_YCBCR = 6
};

enum IMGMAP_FILETYPE {
  IMGMAPFILE_UKN    = 0, // File type needs to be specified
  IMGMAPFILE_PBM    = 1, // PBM/PGM/PPM text and binary files
  IMGMAPFILE_BMP    = 2, // Old Windows format
  IMGMAPFILE_PCX    = 3, // Another old format for DOS/Windows
  IMGMAPFILE_TIFFLE = 4, // Tiff on x86
  IMGMAPFILE_TIFFBE = 5  // Tiff on 68k
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
int imgmap_createGreyScaleImg(IMGMAP_FILE *fmap, const char *name, int mode,
    int file_type, int sx, int sy, int max_val);
int imgmap_createRGBImg(IMGMAP_FILE *fmap, const char *name, int mode,
    int file_type, int sx, int sy, int max_val);
int imgmap_createBitmapImg(IMGMAP_FILE *fmap, const char *name, int mode,
    int file_type, int sx, int sy, int num_colors);

#endif

