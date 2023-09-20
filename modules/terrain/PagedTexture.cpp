#include "PagedTexture.hpp"
#include "core/error/error_macros.h"
#include "core/math/math_funcs.h"
#include "core/variant/variant.h"
#include <intsafe.h>
#include <stdint.h>
#include <stdio.h>
#include <vector>

namespace terrain
{

int64_t PagedTexture::GetPageStartPos(int mipmapLevel, int pageX, int pageY) const
{
    int pixelSize = _nChannel * _bitDepth / 8;
    int64_t pos = _headerSize;

    for (int i = 0; i < mipmapLevel; ++i)
    {
        int width = (_width >> i) + 2 * Math::ceil((_width >> i) / (float)_tileWidth) * _borderWidth;
        int height = (_height >> i) + 2 * Math::ceil((_height >> i) / (float)_tileWidth) * _borderWidth;
        pos += width * height * pixelSize;
    }

    int nPageX = Math::ceil((float)(_width >> mipmapLevel) / _tileWidth);
    int nPageY = Math::ceil((float)(_height >> mipmapLevel) / _tileWidth);

    if (pageX >= nPageX || pageX < 0)
    {
        ERR_FAIL_V_MSG(-1, "invalid tileX.");
    }

    if (pageY >= nPageY || pageY < 0)
    {
        ERR_FAIL_V_MSG(-1, "invalid tileY.");
    }

    int pageWidth = _tileWidth + 2 * _borderWidth;

    int edgePageWidth = nPageX * pageWidth - (nPageX - 1) * pageWidth;
    int edgePageHeight = nPageY * pageWidth - (nPageY - 1) * pageWidth;

    pos += (int64_t)pageY * (nPageX - 1) * pageWidth * pageWidth * pixelSize; // except the last column and row.
    pos += (int64_t)pageY * edgePageWidth * pageWidth * pixelSize; // except the last row in the last column.

    if (pageY == nPageY - 1)
    {
        pos += (int64_t)pageX * edgePageHeight * pageWidth * pixelSize;
    }
    else
    {
        pos += (int64_t)pageX * pageWidth * pageWidth * pixelSize;
    }

    return pos;
}

PagedTexture::PagedTexture()
{
}

PagedTexture::PagedTexture(PagedTexture &&other) : _file(std::move(other._file))
{
    _width = other._width;
    _height = other._height;
    _mipmapCount = other._mipmapCount;
    _nChannel = other._nChannel;
    _bitDepth = other._bitDepth;
}

bool PagedTexture::Open(const char *fileName)
{
    if (!_file.Open(fileName, ReadOnly))
    {
        ERR_FAIL_V_MSG(false, vformat("can't open file %s", fileName));
    }

    // read header
    _width = _file.Read16();
    _height = _file.Read16();
    _tileWidth = _file.Read16();
    _bitDepth = _file.Read8();
    _nChannel = _file.Read8();
    _mipmapCount = _file.Read8();
    _borderWidth = _file.Read8();

    return true;
}

PagedTexture::Page PagedTexture::LoadPage(int mipmapLevel, int tileX, int tileY)
{
    Page tile;

    if (mipmapLevel >= _mipmapCount || mipmapLevel < 0)
    {
        ERR_FAIL_V_MSG(tile, "invalid mipmapLevel.");
    }

    int64_t offset = GetPageStartPos(mipmapLevel, tileX, tileY);
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

    int pixelSize = _nChannel * _bitDepth;

    tile.data = _file.Read(width * height * pixelSize);
    tile.width = width;
    tile.height = height;
    return tile;
}
} // namespace terrain