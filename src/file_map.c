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
#include <stdint.h>
#include "file_map.h"

inline static int _openflags(int mode) {
  switch(mode) {
    case IMGMAP_READPRIVATE:
    case IMGMAP_READSHARED:
      return O_RDONLY;
    default:
      return O_RDWR;
  }
}

inline static int _protflags(int mode) {
  switch(mode) {
    case IMGMAP_READPRIVATE:
    case IMGMAP_READSHARED:
      return PROT_READ;
    default:
      return PROT_READ | PROT_WRITE;
  }
}

inline static int _mapflags(int mode) {
  switch(mode) {
    case IMGMAP_ANONYMOUS:
      return MAP_PRIVATE | MAP_ANONYMOUS;
    case IMGMAP_READPRIVATE:
    case IMGMAP_RWPRIVATE:
      return MAP_PRIVATE;
    default:
      return MAP_SHARED;
  }
}

inline static int _syncflags(int mode) {
  switch(mode) {
    case IMGMAP_ASYNC:
      return MS_ASYNC;
    case IMGMAP_SYNC:
      return MS_SYNC;
    case IMGMAP_INVALIDATE:
      return MS_INVALIDATE;
    default:
      return -1;
  }
}

int imgmap_unmap(IMGMAP_FILE *fmap) {
  int ret = munmap(fmap->map, fmap->map_size);
  fmap->map = NULL;
  return ret;
}

int imgmap_memSync(void *begin, void *end, int mode) {
  int m = _syncflags(mode);
  if(m<0)
    return -1;
  uintptr_t p = (uintptr_t) begin;
  p = (p/getpagesize())*getpagesize();
  size_t len = ((uintptr_t) end) - p;
  return msync((void*) p, len, m);
}

int imgmap_mapBuffer(IMGMAP_FILE *fmap, size_t size, int mode, int file_id) {
  size_t page_size = (size_t) getpagesize();
  fmap->map_size = ((size-1)/page_size+1)*page_size;
    fmap->map = mmap(NULL, fmap->map_size, _protflags(mode),
      _mapflags(mode), file_id, 0);
  if(fmap->map == NULL)
    return IMGMAP_EMMAP;
  fmap->mode = mode;
  fmap->end = (void*) ((char*) fmap->map) + size;
  fmap->file_id = -1; // useful when the file is opened by the program
  return IMGMAP_OK;
}

// Interface for loading by file_id instead of path
int imgmap_loadMap(IMGMAP_FILE *fmap, int file_id, int mode) {
  struct stat statbuf;
  if(fstat(file_id, &statbuf) < 0)
    return IMGMAP_ESTATFILE;
  if(statbuf.st_size <= 4)
    return IMGMAP_EINVALIDFILE;
  return imgmap_mapBuffer(fmap, statbuf.st_size, mode, file_id);
}

int imgmap_loadMapFile(IMGMAP_FILE *fmap, const char *name, int mode) {
  int file_id = open(name, _openflags(mode));
  if(file_id < -1)
    return IMGMAP_EOPENFILE;
  int ret = imgmap_loadMap(fmap, file_id, mode);
  if(ret < 0)
    close(file_id); // Error, so close the file
  else
    fmap->file_id = file_id; // Handle the file closing afterwards
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
  if(ret < 0)
    close(file_id); // Error, so close the file
  else
    fmap->file_id = file_id; // Handle the file closing afterwards
  return ret;
}



