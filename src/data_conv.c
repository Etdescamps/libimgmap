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

#ifdef __GNUC__
#  if __GNUC_PREREQ (4, 8)
#    define DATA_CONV_BSWAP16__ __builtin_bswap16
#  endif
#elif defined(__clang__)
#  if __has_builtin(__builtin_swap16)
#    define DATA_CONV_BSWAP16__ __builtin_bswap16
#  endif
#endif


static inline uint16_t _bswap16 (uint16_t x)
{
#ifdef DATA_CONV_BSWAP16__
  return DATA_CONV_BSWAP16__(x);
#else
  return ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8));
#endif
}


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

void imgmap_convInvShortToFloat(float *val, const unsigned short *p,
    size_t size, int max_val) {
  size_t i;
  float ratio = 1.0/((float) max_val);
  for(i=0; i<size; i++) {
    val[i] = ((float) _bswap16(p[i]))*ratio;
  }
}

void imgmap_convFloatToInvShort(unsigned short *val, const float *p,
    size_t size, int max_val) {
  size_t i;
  float mv = max_val;
  for(i=0; i<size; i++) {
    val[i] = _bswap16((unsigned short) mv*p[i]);
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

void imgmap_convPBMToFloat(float *dest, const char *src,
    size_t nlines, size_t sx) {
  size_t lx = sx/8, rx = sx%8;
  size_t dx = rx ? lx+1 : lx;
  size_t i, j, k;
  for(i=0; i<nlines; ++i) {
    const char *v = &src[i*dx];
    size_t l = i*sx;
    for(j=0; j<lx; ++j) {
      char b = v[j];
      for(k=0; k<8; ++k) {
        dest[l++] = b & 0x80 ? 1.0 : 0.0;
        b <<= 1;
      }
    }
    if(rx) {
      char b = v[lx];
      for(k=0; k<rx; ++k) {
        dest[l++] = b & 0x80 ? 1.0 : 0.0;
        b <<= 1;
      }
    }
  }
}


