#include "TiledBitmap.hpp"
#include "core/error/error_macros.h"
#include "core/math/math_funcs.h"
#include "core/variant/variant.h"
#include <intsafe.h>
#include <stdint.h>
#include <stdio.h>
#include <vector>

namespace terrain
{

inline TiledBitmapFormat DetermineFormat(int nChannel, int bitDepth)
{
    if (nChannel == 4)
    {
        return RGBA8UInt;
    }

    if (nChannel == 2)
    {
        return RG8UInt;
    }

    if (nChannel == 1)
    {
        if (bitDepth == 32)
        {
            return R32SFloat;
        }

        if (bitDepth == 16)
        {
            return R16UInt;
        }

        if (nChannel == 8)
        {
            return R8UInt;
        }
    }

    return Unkown;
}

inline void GetPixelInfo(TiledBitmapFormat format, int *nchannel, int *depth)
{
    switch (format)
    {
    case R8UInt:
        *nchannel = 1;
        *depth = 1;
        break;
    case RG8UInt:
        *nchannel = 2;
        *depth = 1;
        break;
    case RGBA8UInt:
        *nchannel = 4;
        *depth = 1;
        break;
    case R16UInt:
        *nchannel = 1;
        *depth = 2;
        break;
    case R32SFloat:
        *nchannel = 1;
        *depth = 4;
        break;
    default:
        break;
    }
}

int64_t TiledBitmap::GetTileStartPos(int mipmapLevel, int tileX, int tileY) const
{
    int pixelSize;
    {
        int nChannel, depth;
        GetPixelInfo(_format, &nChannel, &depth);
        pixelSize = nChannel * depth;
    }

    int64_t pos = _headerSize;

    for (int i = 0; i < mipmapLevel; ++i)
    {
        int width = _width >> i;
        int height = _height >> i;
        pos += width * height * pixelSize;
    }

    int mipWidth = _width >> mipmapLevel;
    int mipHeight = _height >> mipmapLevel;

    int nTileX = Math::ceil((float)mipWidth / _tileWidth);
    int nTileY = Math::ceil((float)mipHeight / _tileWidth);

    if (tileX >= nTileX || tileX < 0)
    {
        ERR_FAIL_V_MSG(-1, "invalid tileX.");
    }

    if (tileY >= nTileY || tileY < 0)
    {
        ERR_FAIL_V_MSG(-1, "invalid tileY.");
    }

    int edgeTileWidth = nTileX * _tileWidth - (nTileX - 1) * _tileWidth;
    int edgeTileHeight = nTileY * _tileWidth - (nTileY - 1) * _tileWidth;

    pos += (int64_t)tileY * (nTileX - 1) * _tileWidth * _tileWidth * pixelSize; // except the last column and row.
    pos += (int64_t)tileY * edgeTileWidth * _tileWidth * pixelSize; // except the last row in the last column.

    if (tileY == nTileY - 1)
    {
        pos += (int64_t)tileX * edgeTileHeight * _tileWidth * pixelSize;
    }
    else
    {
        pos += (int64_t)tileX * _tileWidth * _tileWidth * pixelSize;
    }

    return pos;
}

TiledBitmap::TiledBitmap()
{
}

TiledBitmap::TiledBitmap(TiledBitmap &&other) : _file(std::move(other._file))
{
    _width = other._width;
    _height = other._height;
    _mipmapCount = other._mipmapCount;
    _format = other._format;
}

bool TiledBitmap::Open(const char *fileName)
{
    if (!_file.Open(fileName, ReadOnly))
    {
        ERR_FAIL_V_MSG(false, vformat("can't open file %s", fileName));
    }

    // read header
    _width = _file.Read16();
    _height = _file.Read16();
    _tileWidth = _file.Read16();
    int bitDepth = _file.Read8();
    int nChannel = _file.Read8();
    _mipmapCount = _file.Read8();

    _format = DetermineFormat(nChannel, bitDepth);

    return true;
}

TiledBitmap::Tile TiledBitmap::LoadTile(int mipmapLevel, int tileX, int tileY)
{
    Tile tile;

    if (mipmapLevel >= _mipmapCount || mipmapLevel < 0)
    {
        ERR_FAIL_V_MSG(tile, "invalid mipmapLevel.");
    }

    int64_t offset = GetTileStartPos(mipmapLevel, tileX, tileY);
    if (!_file.Seek(offset))
    {
        ERR_FAIL_V_MSG(tile, "set seek failed.");
    }

    int width;
    int height;
    {
        int nTileX = Math::ceil((float)(_width >> mipmapLevel) / _tileWidth);
        int nTileY = Math::ceil((float)(_height >> mipmapLevel) / _tileWidth);

        width = tileX == nTileX - 1 ? nTileX * _tileWidth - (nTileX - 1) * _tileWidth : _tileWidth;
        height = tileY == nTileY - 1 ? nTileY * _tileWidth - (nTileY - 1) * _tileWidth : _tileWidth;
    }

    int pixelSize;
    {
        int nChannel, depth;
        GetPixelInfo(_format, &nChannel, &depth);
        pixelSize = nChannel * depth;
    }

    tile.data = _file.Read(width * height * pixelSize);
    tile.width = width;
    tile.height = height;
    return tile;
}
} // namespace terrain