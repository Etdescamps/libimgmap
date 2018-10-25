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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "file_map.h"

static int _openflags(int mode) {
  switch(mode) {
    case IMGMAP_READPRIVATE:
    case IMGMAP_READSHARED:
      return O_RDONLY;
    default:
      return O_RDWR;
  }
}

static int _protflags(int mode) {
  switch(mode) {
    case IMGMAP_READPRIVATE:
    case IMGMAP_READSHARED:
      return PROT_READ;
    default:
      return PROT_READ | PROT_WRITE;
  }
}

static int _mapflags(int mode) {
  switch(mode) {
    case IMGMAP_READPRIVATE:
    case IMGMAP_RWPRIVATE:
      return MAP_SHARED;
    default:
      return MAP_PRIVATE;
  }
}

// Interface for loading by file_id instead of path
int imgmap_loadMap(IMGMAP_FILE *fmap, int file_id, int mode) {
  struct stat statbuf;
  if(fstat(file_id, &statbuf) < 0)
    return IMGMAP_ESTATFILE;
  if(statbuf.st_size <= 4)
    return IMGMAP_EINVALIDFILE;
  size_t page_size = (size_t) getpagesize();
  size_t data_size = statbuf.st_size;
  fmap->map_size = ((data_size-1)/page_size+1)*page_size;
  fmap->map = mmap(NULL, fmap->map_size, _protflags(mode),
      _mapflags(mode), file_id, 0);
  if(fmap->map == NULL)
    return IMGMAP_EMMAP;
  fmap->mode = mode;
  fmap->end = (void*) ((char*) fmap->map) + data_size;
  return IMGMAP_OK;
}

int imgmap_loadMapFile(IMGMAP_FILE *fmap, const char *name, int mode) {
  int file_id = open(name, _openflags(mode));
  if(file_id < -1)
    return IMGMAP_EOPENFILE;
  int ret = imgmap_loadMap(fmap, file_id, mode);
  // Mapped files can be closed (they are still referenced)
  close(file_id);
  return ret;
}

static int _resizeFile(IMGMAP_FILE *fmap, size_t size,
    int file_id, int mode) {
  int ret = ftruncate(file_id, size);
  if(ret<0)
    return IMGMAP_ETRUNCATE;
  return imgmap_loadMap(fmap, file_id, mode);
}

int imgmap_createFile(IMGMAP_FILE *fmap, size_t size, 
    const char *name, int mode) {
  if(mode != IMGMAP_RWPRIVATE && mode != IMGMAP_RWSHARED)
    return IMGMAP_EINVALIDMODE;
  int file_id = open(name, O_RDWR | O_TRUNC | O_CREAT,
      S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if(file_id < -1)
    return IMGMAP_EOPENFILE;
  int ret = _resizeFile(fmap, size, file_id, mode);
  // Mapped files can be closed (they are still referenced)
  close(file_id);
  return ret;
}



