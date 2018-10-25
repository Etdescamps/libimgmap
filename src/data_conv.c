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

#include "data_conv.h"

// TODO: more optimized version using SIMD
void imgmap_convByteToFloat(float *val, const char *p, size_t size,
    int max_val) {
  size_t i;
  float ratio = 1.0/((float) max_val);
  for(i=0; i<size; i++) {
    val[i] = ((float) p[i])*ratio;
  }
}

void imgmap_convShortToFloat(float *val, const unsigned short *p,
    size_t size, int max_val) {
  size_t i;
  float ratio = 1.0/((float) max_val);
  for(i=0; i<size; i++) {
    val[i] = ((float) p[i])*ratio;
  }
}

void imgmap_convFloatToShort(unsigned short *val, const float *p,
    size_t size, int max_val) {
  size_t i;
  float mv = max_val;
  for(i=0; i<size; i++) {
    val[i] = (unsigned short) mv*p[i];
  }
}

void imgmap_convFloatToByte(char *val, const float *p,
    size_t size, int max_val) {
  size_t i;
  float mv = max_val;
  for(i=0; i<size; i++) {
    val[i] = (char) mv*p[i];
  }
}

