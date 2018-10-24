#include "file_parse.h"
#include "file_map.h"

static int _pbmNextInt(const char **p, const char *end) {
  int ret = IMGMAP_OK;
  for(;;) {
    ret = imgmap_parse_space(p, end);
    if(ret < IMGMAP_OK)
      return ret;
    // Comment section
    if(**p == '#')
      return imgmap_parse_moveNextLine(p, end);
    else // Shall be another the first character of an integer
      return imgmap_parse_readPosInt(p, end);
  }
}

int imgmap_pgm_readHeader(IMGMAP_FILE *fmap) {
  const char *p = (const char *) fmap->map;
  const char *end = (const char *) fmap->end;
  int type = p[1] - '0';
  p += 2;
  // Read sx and sy
  if((fmap->sx = _pbmNextInt(&p,end)) < 0)
    return IMGMAP_EPARSE;
  if((fmap->sy = _pbmNextInt(&p,end)) < 0)
    return IMGMAP_EPARSE;
  if(type != 1 && type !=4)  { // read max_value field of grey/RGB image
    if((fmap->max_val = _pbmNextInt(&p,end)) < 0)
      return IMGMAP_EPARSE;
  }
  else // B&W image do not have max_value field (max_val == 1)
    fmap->max_val = 1;
  ++p; // Ignore one space character
  fmap->data = (void*) p;
  // Configure fmap in order to match the file structure

  fmap->palette = NULL; // No palette support on this type
  fmap->nl = 1; // Supports only one layer

  // Determine number of channels and palette_type
  if(type == 3 || type == 6) {
    // RGB images
    fmap->nc = 3;
    fmap->palette_type = IMGMAP_RGB;
  }
  else {
    // B&W and greyscale images
    fmap->nc = 1;
    if(type == 1 || type == 4) // B&W
      fmap->palette_type = IMGMAP_BW;
    else // Grey
      fmap->palette_type = IMGMAP_GREY;
  }

  // Determine data_type and dx
  if(type <= 3) {
    fmap->data_type = IMGMAP_TEXTPBM;
    fmap->dx = -1; // Not applicable: text file
  }
  if(type == 4) {
    fmap->data_type = IMGMAP_RAW1BPP;
    fmap->dx = ((fmap->sx-1)/8+1);
  }
  else  { // (type == 5 || type == 6)
    if(fmap->max_val < 256) {
      fmap->data_type = IMGMAP_RAW1BYTE;
      fmap->dx = fmap->sx*fmap->nc;
    }
    else { // Extension up to 65535 using two bytes
      fmap->data_type = IMGMAP_RAW2BYTEBE;
      fmap->dx = 2*fmap->sx*fmap->nc;
    }
  }
  return IMGMAP_OK;
}

