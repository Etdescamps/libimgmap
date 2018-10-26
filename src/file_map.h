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

#ifndef IMGMAP_FILE_MAP_H__
#define IMGMAP_FILE_MAP_H__
#include "imgmap.h"

int imgmap_loadMapId(IMGMAP_FILE *fmap, int file_id, int mode);
int imgmap_loadMapFile(IMGMAP_FILE *fmap, const char *name, int mode);

int imgmap_mapBuffer(IMGMAP_FILE *fmap, size_t size, int mode, int file_id);

int imgmap_createFile(IMGMAP_FILE *fmap, size_t size,
    const char *name, int mode);
int imgmap_memSync(void *begin, void *end, int mode);
int imgmap_unmap(IMGMAP_FILE *fmap);

#endif //IMGMAP_FILE_MAP_H__

