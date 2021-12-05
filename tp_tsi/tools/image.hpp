// image.h

#ifndef IMAGE_H
#define IMAGE_H

#include <string>

enum ImageType
{
  IMAGE_TYPE_GRAY,
  IMAGE_TYPE_GRAYA,
  IMAGE_TYPE_RGB,
  IMAGE_TYPE_RGBA
};

struct Image
{
  int            width;
  int            height;
  ImageType      type;
  unsigned char *data;
  ~Image() { delete[] data; }
};

Image *image_load_tga(const std::string &filename);

#endif
