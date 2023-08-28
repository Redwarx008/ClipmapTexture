
#include "clipmap.hpp"

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

#include "constants/terrain_constants.hpp"

namespace terrain
{

Clipmap::~Clipmap()
{
}

std::unique_ptr<Clipmap> Clipmap::Create(const char *fileName, const Vector2i &center)
{
    File file;
    if (!file.Open(fileName, ReadOnly))
    {
        ERR_FAIL_V_MSG(nullptr, vformat("can't open file %s", fileName));
    }

    auto ret = std::unique_ptr<Clipmap>(new Clipmap(std::move(file), center));
    return std::move(ret);
}

Clipmap::Clipmap(File &&file, const Vector2i &center) : _file(std::move(file))
{
    // RenderingDevice* rd =
    // RenderingServer::get_singleton()->get_rendering_device();
    _clipWindowSize = CLIP_WINDOW_SIZE;

    _width = _file.Read16();
    _height = _file.Read16();
    _bitDepth = _file.Read8();
    _nChannel = _file.Read8();
    _nLevel = _file.Read8();

    int headerSize = 2 + 2 + 1 + 1 + 1;

    size_t offset = headerSize;
    for (int i = 0; i < _nLevel; ++i)
    {
        _clipStack.emplace_back(ClipMipmap(CLIP_CACHE_SIZE, _nChannel, _bitDepth));
        _mipmapOffset.push_back(offset);

        offset += (_width >> i) * (_height >> i) * _nChannel * _bitDepth / 8;
    }
}

Clipmap::Clipmap(Clipmap &&other)
    : _file(std::move(other._file)), _mipmapOffset(std::move(other._mipmapOffset)),
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
