#ifndef TILEDBITMAP_H
#define TILEDBITMAP_H

#include "utility/File.hpp"
#include <stdint.h>
#include <vector>

namespace terrain
{

enum TiledBitmapFormat : uint8_t
{
    R8UInt,
    RG8UInt,
    RGBA8UInt,
    R16UInt,
    R32SFloat,
    Unkown
};

class TiledBitmap
{
  public:
    struct Tile
    {
        int width = 0;
        int height = 0;
        std::vector<uint8_t> data;
    };

    TiledBitmap();
    TiledBitmap(TiledBitmap &&other);
    bool Open(const char *fileName);
    Tile LoadTile(int mipmapLevel, int tileX, int tileY);
    int GetWidth() const;
    int GetHeight() const;
    int GetMipmapCout() const;
    TiledBitmapFormat GetFormat() const;

  private:
    int64_t GetTileStartPos(int mipmapLevel, int tileX, int tileY) const;
    const int _headerSize = 9;
    int _width;
    int _height;
    int _tileWidth;
    int _mipmapCount;

    TiledBitmapFormat _format;
    File _file;
};
} // namespace terrain

#endif