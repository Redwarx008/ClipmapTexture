
#include "Clipmap.hpp"

#include <corecrt_wstdio.h>
#include <intsafe.h>
#include <memory>
#include <stdio.h>
#include <utility>

#include "core/error/error_macros.h"
#include "core/io/file_access.h"
#include "core/variant/variant.h"
#include "servers/rendering/rendering_device.h"
#include "servers/rendering_server.h"

#include "constants/TerrainConstants.hpp"

namespace terrain
{

Clipmap::~Clipmap()
{
}

std::unique_ptr<Clipmap> Clipmap::Create(const char* fileName, const Vector2i &center)
{
    TiledBitmap tiledBitmap;
    if (!tiledBitmap.Open(fileName))
    {
        ERR_FAIL_V_MSG(nullptr, vformat("can't open TiledBitmap %s", fileName));
    }

    auto ret = std::unique_ptr<Clipmap>(new Clipmap(std::move(tiledBitmap), center));
    return std::move(ret);
}

Clipmap::Clipmap(TiledBitmap &&tiledBitmap, const Vector2i &center) : _tiledBitmap(std::move(tiledBitmap))
{
    // RenderingDevice* rd =
    // RenderingServer::get_singleton()->get_rendering_device();
    _clipWindowSize = CLIP_WINDOW_SIZE;

    int headerSize = 2 + 2 + 1 + 1 + 1;

    size_t offset = headerSize;
    for (int i = 0; i < _nLevel; ++i)
    {
        _clipStack.emplace_back(ClipLevel(CLIP_CACHE_SIZE, _nChannel, _bitDepth));
        _mipmapOffset.push_back(offset);

        offset += (_width >> i) * (_height >> i) * _nChannel * _bitDepth / 8;
    }
}

Clipmap::Clipmap(Clipmap &&other)
    : _tiledBitmap(std::move(other._tiledBitmap)), _mipmapOffset(std::move(other._mipmapOffset)),
      _clipStack(std::move(other._clipStack))
{
    _width = other._width;
    _height = other._height;
    _nChannel = other._nChannel;
    _bitDepth = other._bitDepth;
    _nLevel = other._nLevel;
    _clipWindowSize = other._clipWindowSize;
    _clipCenter = other._clipCenter;
}

void Clipmap::Update(const Vector2i &newCenter)
{
    
}

} // namespace terrain
