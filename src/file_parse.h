#ifndef LIBIMGMAP_FILE_PARSE_H__
#define LIBIMGMAP_FILE_PARSE_H__
#include "imgmap.h"

// Parse API using mmap file model (the whole file is in memory).
// The function calls update the index position idx in the buffer,
// and return IMGMAP_OK or an error code (IMGMAP_EPARSE).

// Move forward til next non space character
inline static int imgmap_parse_space(const char **p, const char *end) {
  for(;*p < end; ++(*p)) {
    switch(**p) {
      // White spaces
      case ' ':
      case '\n':
      case '\r':
      case '\t':
        break;
      default:
        return IMGMAP_OK;
    }
  }
  // Error: end of file
  return IMGMAP_EPARSE;
}

// Move to the next line (after a line comment section '#' '//' '!' etc.)
inline static int imgmap_parse_moveNextLine(const char **p, const char *end) {
  for(;*p < end; ++(*p)) {
    if(**p == '\n' || **p == '\r')
      return IMGMAP_OK;
  }
  return IMGMAP_EPARSE;
}

inline static int imgmap_parse_readByte(const char **p, const char *end) {
  if(*p >= end)
    return -1;
  int ret = **p;
  ++(*p);
  return ret;
}

inline static int imgmap_parse_readShortBE(const char **p, const char *end) {
  if(*p >= end-1)
    return -1;
  int ret = (*p)[0] << 8 | (*p)[1];
  *p += 2;
  return ret;
}

// Read a positive integer (return a negative integer if an error occurs) 
int imgmap_parse_readPosInt(const char **p, const char *end);

int imgmap_parse_readNTextByte(char *val, size_t size,
    const char **p, const char *end);

int imgmap_parse_readNTextShort(short *val, size_t size,
    const char **p, const char *end);

#endif //LIBIMGMAP_TEXT_PARSE_H__

