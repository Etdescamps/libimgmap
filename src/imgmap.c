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

#include <sys/mman.h>
#include "imgmap.h"
#include "file_map.h"
#include "file_types.h"

#define TEXT_BUFFER_SIZE__ 256

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

int imgmap_createGreyScaleImg(IMGMAP_FILE *fmap, const char *name, int mode,
    int file_type, int sx, int sy, int max_val) {
  fmap->sx = sx;
  fmap->sy = sy;
  fmap->nc = 1;
  switch(file_type) {
    case IMGMAPFILE_PBM:
      return imgmap_createPBM(fmap, name, mode, 5, max_val);
  }
  return IMGMAP_ENOTSUPPORTED;
}
