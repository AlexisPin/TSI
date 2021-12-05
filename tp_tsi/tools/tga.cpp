// use STB for windows compatibility
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "image.hpp"
#include <fstream>

Image *image_load_tga(const std::string& filename)
{
  std::ifstream file(filename);
  if(!file.is_open()){return nullptr;} 

  Image* im=new Image;
  int type;
  im->data = stbi_load(filename.c_str(), &im->width, &im->height, &type, 3);
  im->type = IMAGE_TYPE_RGB;
  return im;
}


///* tga.c */
//
//#include <stdlib.h>
//#include <stdio.h>
//#include <errno.h>
//#include <string.h>
//
//#include "image.hpp"
//
///* Round up a division to the nearest integer. */
//#define ROUNDUP_DIVIDE(n,d) (((n) + (d - 1)) / (d))
//
///* TRUEVISION-XFILE magic signature string */
//static unsigned char magic[18] =
//{
//  0x54, 0x52, 0x55, 0x45, 0x56, 0x49, 0x53, 0x49, 0x4f,
//  0x4e, 0x2d, 0x58, 0x46, 0x49, 0x4c, 0x45, 0x2e, 0x0
//};
//
//typedef struct tga_info_struct
//{
//  unsigned char idLength;
//  unsigned char colorMapType;
//
//  unsigned char imageType;
//  /* Known image types. */
//#define TGA_TYPE_MAPPED      1
//#define TGA_TYPE_COLOR       2
//#define TGA_TYPE_GRAY        3
//
//  unsigned char imageCompression;
//  /* Only known compression is RLE */
//#define TGA_COMP_NONE        0
//#define TGA_COMP_RLE         1
//
//  /* Color Map Specification. */
//  /* We need to separately specify high and low bytes to avoid endianness
//     and alignment problems. */
//
//  unsigned short colorMapIndex;
//  unsigned short colorMapLength;
//  unsigned char  colorMapSize;
//
//  /* Image Specification. */
//  unsigned short xOrigin;
//  unsigned short yOrigin;
//
//  unsigned short width;
//  unsigned short height;
//
//  unsigned char bpp;
//  unsigned char bytes;
//
//  unsigned char alphaBits;
//  unsigned char flipHoriz;
//  unsigned char flipVert;
//
//  /* Extensions (version 2) */
//
//  /* Not all the structures described in the standard are transcribed here
//     only those which seem applicable to Gimp */
//
//  char         authorName[41];
//  char         comment[324];
//  unsigned int month, day, year, hour, minute, second;
//  char         jobName[41];
//  char         softwareID[41];
//  unsigned int pixelWidth, pixelHeight;  /* write dpi? */
//  double       gamma;
//} tga_info;
//
//
///* Declare some local functions.
//*/
//static Image *ReadImage (FILE       *fp,
//    tga_info   *info);
//
//
//  Image *
//image_load_tga (const std::string &filename)
//{
//  FILE          *fp;
//  tga_info       info;
//  unsigned char  header[18];
//  unsigned char  footer[26];
//  unsigned char  extension[495];
//  long           offset;
//  Image         *image;
//
//  fp = fopen (filename.c_str (), "r");
//  if (!fp)
//  {
//    printf ("Could not open '%s' for reading: %s\n", filename.c_str (), strerror (errno));
//    return NULL;
//  }
//
//  if (!fseek (fp, -26L, SEEK_END)) { /* Is file big enough for a footer? */
//    if (fread (footer, sizeof (footer), 1, fp) != 1)
//    {
//      printf ("Cannot read footer from '%s'\n", filename.c_str ());
//      return NULL;
//    }
//    else if (memcmp (footer + 8, magic, sizeof (magic)) == 0)
//    {
//      /* Check the signature. */
//
//      offset= footer[0] + (footer[1] * 256) + (footer[2] * 65536)
//        + (footer[3] * 16777216);
//
//      if (offset != 0)
//      {
//        if (fseek (fp, offset, SEEK_SET) ||
//            fread (extension, sizeof (extension), 1, fp) != 1)
//        {
//          printf ("Cannot read extension from '%s'\n", filename.c_str ());
//          return NULL;
//        }
//        /* Eventually actually handle version 2 TGA here */
//      }
//    }
//  }
//
//  if (fseek (fp, 0, SEEK_SET) ||
//      fread (header, sizeof (header), 1, fp) != 1)
//  {
//    printf ("Cannot read header from '%s'\n", filename.c_str ());
//    return NULL;
//  }
//
//  switch (header[2])
//  {
//    case 1:
//      info.imageType = TGA_TYPE_MAPPED;
//      info.imageCompression = TGA_COMP_NONE;
//      break;
//    case 2:
//      info.imageType = TGA_TYPE_COLOR;
//      info.imageCompression = TGA_COMP_NONE;
//      break;
//    case 3:
//      info.imageType = TGA_TYPE_GRAY;
//      info.imageCompression = TGA_COMP_NONE;
//      break;
//
//    case 9:
//      info.imageType = TGA_TYPE_MAPPED;
//      info.imageCompression = TGA_COMP_RLE;
//      break;
//    case 10:
//      info.imageType = TGA_TYPE_COLOR;
//      info.imageCompression = TGA_COMP_RLE;
//      break;
//    case 11:
//      info.imageType = TGA_TYPE_GRAY;
//      info.imageCompression = TGA_COMP_RLE;
//      break;
//
//    default:
//      info.imageType = 0;
//  }
//
//  info.idLength     = header[0];
//  info.colorMapType = header[1];
//
//  info.colorMapIndex  = header[3] + header[4] * 256;
//  info.colorMapLength = header[5] + header[6] * 256;
//  info.colorMapSize   = header[7];
//
//  info.xOrigin = header[8] + header[9] * 256;
//  info.yOrigin = header[10] + header[11] * 256;
//  info.width   = header[12] + header[13] * 256;
//  info.height  = header[14] + header[15] * 256;
//
//  info.bpp       = header[16];
//  info.bytes     = (info.bpp + 7) / 8;
//  info.alphaBits = header[17] & 0x0f; /* Just the low 4 bits */
//  info.flipHoriz = (header[17] & 0x10) ? 1 : 0;
//  info.flipVert  = (header[17] & 0x20) ? 0 : 1;
//
//  /* hack to handle some existing files with incorrect headers, see bug #306675 */
//  if (info.alphaBits == info.bpp)
//    info.alphaBits = 0;
//
//  switch (info.imageType)
//  {
//    case TGA_TYPE_MAPPED:
//      if (info.bpp != 8)
//      {
//        printf ("Unhandled sub-format in '%s'\n", filename.c_str ());
//        return NULL;
//      }
//      break;
//    case TGA_TYPE_COLOR:
//      if (info.bpp != 15 && info.bpp != 16 && info.bpp != 24
//          && info.bpp != 32)
//      {
//        printf ("Unhandled sub-format in '%s'\n", filename.c_str ());
//        return NULL;
//      }
//      break;
//    case TGA_TYPE_GRAY:
//      if (info.bpp != 8 && (info.alphaBits != 8 || (info.bpp != 16 && info.bpp != 15)))
//      {
//        printf ("Unhandled sub-format in '%s'\n", filename.c_str ());
//        return NULL;
//      }
//      break;
//
//    default:
//      printf ("Unknown image type for '%s'\n", filename.c_str ());
//      return NULL;
//  }
//
//  /* Plausible but unhandled formats */
//  if (info.bytes * 8 != info.bpp && !(info.bytes == 2 && info.bpp == 15))
//  {
//    puts ("No support yet for TGA with these parameters");
//    return NULL;
//  }
//
//  /* Check that we have a color map only when we need it. */
//  if (info.imageType == TGA_TYPE_MAPPED && info.colorMapType != 1)
//  {
//    printf ("Indexed image has invalid color map type %d\n", info.colorMapType);
//    return NULL;
//  }
//  else if (info.imageType != TGA_TYPE_MAPPED && info.colorMapType != 0)
//  {
//    printf ("Non-indexed image has invalid color map type %d\n", info.colorMapType);
//    return NULL;
//  }
//
//  /* Skip the image ID field. */
//  if (info.idLength && fseek (fp, info.idLength, SEEK_CUR))
//  {
//    printf ("File '%s' is truncated or corrupted\n", filename.c_str ());
//    return NULL;
//  }
//
//  image = ReadImage (fp, &info);
//  fclose (fp);
//  return image;
//}
//
//  static int
//rle_read (FILE          *fp,
//    unsigned char *buffer,
//    tga_info      *info)
//{
//  static int           repeat = 0;
//  static int           direct = 0;
//  static unsigned char sample[4];
//  int head;
//  int x, k;
//
//  for (x = 0; x < info->width; x++)
//  {
//    if (repeat == 0 && direct == 0)
//    {
//      head = getc (fp);
//
//      if (head == EOF)
//      {
//        return EOF;
//      }
//      else if (head >= 128)
//      {
//        repeat = head - 127;
//
//        if (fread (sample, info->bytes, 1, fp) < 1)
//          return EOF;
//      }
//      else
//      {
//        direct = head + 1;
//      }
//    }
//
//    if (repeat > 0)
//    {
//      for (k = 0; k < info->bytes; ++k)
//      {
//        buffer[k] = sample[k];
//      }
//
//      repeat--;
//    }
//    else /* direct > 0 */
//    {
//      if (fread (buffer, info->bytes, 1, fp) < 1)
//        return EOF;
//
//      direct--;
//    }
//
//    buffer += info->bytes;
//  }
//
//  return 0;
//}
//
//  static void
//flip_line (unsigned char *buffer,
//    tga_info      *info)
//{
//  unsigned char  temp;
//  unsigned char *alt;
//  int            x, s;
//
//  alt = buffer + (info->bytes * (info->width - 1));
//
//  for (x = 0; x * 2 <= info->width; x++)
//  {
//    for (s = 0; s < info->bytes; ++s)
//    {
//      temp = buffer[s];
//      buffer[s] = alt[s];
//      alt[s] = temp;
//    }
//
//    buffer += info->bytes;
//    alt -= info->bytes;
//  }
//}
//
///* Some people write 16-bit RGB TGA files. The spec would probably
//   allow 27-bit RGB too, for what it's worth, but I won't fix that
//   unless someone actually provides an existence proof */
//
//  static void
//upsample (unsigned char *dest,
//    unsigned char *src,
//    unsigned int   width,
//    unsigned int   bytes,
//    unsigned char  alphaBits)
//{
//  unsigned int x;
//
//  for (x = 0; x < width; x++)
//  {
//    dest[0] =  ((src[1] << 1) & 0xf8);
//    dest[0] += (dest[0] >> 5);
//
//    dest[1] =  ((src[0] & 0xe0) >> 2) + ((src[1] & 0x03) << 6);
//    dest[1] += (dest[1] >> 5);
//
//    dest[2] =  ((src[0] << 3) & 0xf8);
//    dest[2] += (dest[2] >> 5);
//
//    switch (alphaBits)
//    {
//      case 1:
//        dest[3] = (src[1] & 0x80)? 0: 255;
//        dest += 4;
//        break;
//      default:
//        dest += 3;
//    }
//
//    src += bytes;
//  }
//}
//
//  static void
//bgr2rgb (unsigned char *dest,
//    unsigned char *src,
//    unsigned int   width,
//    unsigned int   bytes,
//    unsigned int   alpha)
//{
//  unsigned int x;
//
//
//  if (alpha)
//  {
//    for (x = 0; x < width; x++)
//    {
//      *(dest++) = src[2];
//      *(dest++) = src[1];
//      *(dest++) = src[0];
//      *(dest++) = src[3];
//
//      src += bytes;
//    }
//  }
//  else
//  {
//    for (x = 0; x < width; x++)
//    {
//      *(dest++) = src[2];
//      *(dest++) = src[1];
//      *(dest++) = src[0];
//
//      src += bytes;
//    }
//  }
//}
//
//  static void
//read_line (FILE          *fp,
//    unsigned char *row,
//    unsigned char *buffer,
//    tga_info      *info)
//{
//  if (info->imageCompression == TGA_COMP_RLE)
//  {
//    rle_read (fp, buffer, info);
//  }
//  else
//  {
//    if(fread (buffer, info->bytes, info->width, fp)<1)
//      perror("Incorrect read");
//  }
//
//  if (info->flipHoriz)
//  {
//    flip_line (buffer, info);
//  }
//
//  if (info->imageType == TGA_TYPE_COLOR)
//  {
//    if (info->bpp == 16 || info->bpp == 15)
//    {
//      upsample (row, buffer, info->width, info->bytes, info->alphaBits);
//    }
//    else
//    {
//      bgr2rgb (row, buffer,info->width,info->bytes, info->alphaBits);
//    }
//  }
//  else
//  {
//    memcpy (row, buffer, info->width * info->bytes);
//  }
//}
//
//  static Image *
//ReadImage (FILE       *fp,
//    tga_info   *info)
//{
//  Image *image;
//
//  unsigned char *buffer, *row;
//  int            y;
//
//  image = new Image;
//
//  switch (info->imageType)
//  {
//    case TGA_TYPE_GRAY:
//      if (info->alphaBits)
//        image->type = IMAGE_TYPE_GRAYA;
//      else
//        image->type = IMAGE_TYPE_GRAY;
//      break;
//
//    case TGA_TYPE_COLOR:
//      if (info->alphaBits)
//        image->type = IMAGE_TYPE_RGBA;
//      else
//        image->type = IMAGE_TYPE_RGB;
//      break;
//  }
//
//  image->width  = info->width;
//  image->height = info->height;
//  image->data = new unsigned char [info->width * info->height * info->bytes];
//  buffer      = new unsigned char [info->width * info->bytes];
//
//  if (info->flipVert)
//  {
//    for (y = 1; y <= info->height; ++y)
//    {
//      row = image->data + (info->width * info->bytes * (info->height - y));
//      read_line (fp, row, buffer, info);
//    }
//  }
//  else
//  {
//    for (y = 0; y < info->height; ++y)
//    {
//      row = image->data + (info->width * info->bytes * y);
//      read_line (fp, row, buffer, info);
//    }
//  }
//
//  delete[] buffer;
//
//  return image;
//}
