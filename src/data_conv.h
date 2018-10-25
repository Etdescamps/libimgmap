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

#ifndef IMGMAP_DATA_CONV_H__
#define IMGMAP_DATA_CONV_H__

#include <stddef.h>
#include <stdint.h>

void imgmap_convByteToFloat(float *val, const unsigned char *p, size_t size,
    int max_val);
void imgmap_convShortToFloat(float *val, const unsigned short *p,
    size_t size, int max_val);
void imgmap_convInvShortToFloat(float *val, const unsigned short *p,
    size_t size, int max_val);
void imgmap_convFloatToShort(unsigned short *val, const float *p,
    size_t size, int max_val);
void imgmap_convFloatToInvShort(unsigned short *val, const float *p,
    size_t size, int max_val);
void imgmap_convFloatToByte(char *val, const float *p,
    size_t size, int max_val);
void imgmap_convPBMToFloat(float *dest, const char *src,
    size_t nlines, size_t sx);

inline static int is_short16LE() {
  union {
    uint16_t i;
    char c[2];
  } x = {1};
  return x.c[0];
}

#endif
