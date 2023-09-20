#ifndef TILEDBITMAP_H
#define TILEDBITMAP_H

#include "utility/File.hpp"
#include <stdint.h>
#include <vector>

namespace terrain
{
class PagedTexture
{
  public:
    struct Page
    {
        int width = 0;
        int height = 0;
        std::vector<uint8_t> data;
    };

    PagedTexture();
    PagedTexture(PagedTexture &&other);
    bool Open(const char *fileName);
    Page LoadPage(int mipmapLevel, int pageX, int pageY);
    int GetWidth() const;
    int GetHeight() const;
    int GetMipmapCout() const;

  private:
    int64_t GetPageStartPos(int mipmapLevel, int pageX, int pageY) const;
    const int _headerSize = 9;
    int _width;
    int _height;
    int _tileWidth;
    int _mipmapCount;
    int _nChannel;
    int _bitDepth;
    int _borderWidth;
    File _file;
};
} // namespace terrain

#endif