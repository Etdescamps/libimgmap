#include "file_parse.h"

inline static int _readPosInt(const char **p, const char *end) {
  if(**p < '0' || **p > '9') {
    // Error: not an integer token
    return -1;
  }
  int r_int = **p-'0';
  for(++(*p); *p < end; ++(*p)) {
    if(**p < '0' || **p > '9')
      break;
    r_int = 10*r_int+(**p-'0');
  }
  return r_int;
}

int imgmap_parse_readPosInt(const char **p, const char *end) {
  return _readPosInt(p, end);
}

int imgmap_parse_readNTextByte(char *val, size_t size,
    const char **p, const char *end) {
  int i;
  for(i = 0; i < size; i++) {
    int k = _readPosInt(p, end);
    if(k<0 || k > 255)
      return i;
    val[i] = k;
  }
  return i;
}

int imgmap_parse_readNTextShort(short *val, size_t size,
    const char **p, const char *end) {
  int i;
  for(i = 0; i < size; i++) {
    if(imgmap_parse_space(p, end)<0)
      return i;
    int k = _readPosInt(p, end);
    if(k<0 || k > 65535)
      return i;
    val[i] = k;
  }
  return i;
}

