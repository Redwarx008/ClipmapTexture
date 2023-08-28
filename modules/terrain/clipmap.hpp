#ifndef CLIPMAP_H
#define CLIPMAP_H

#include "core/math/rect2i.h"
#include "core/math/vector2i.h"
#include "core/templates/rid.h"
#include <intsafe.h>
#include <memory>
#include <stdint.h>
#include <vector>

#include "Utility/file.hpp"
#include "constants/terrain_constants.hpp"
#include "clip_mipmap.hpp"

namespace terrain
{

class Clipmap
{
  public:
    Clipmap(Clipmap &&other);
    Clipmap(const Clipmap &) = delete;
    Clipmap &operator=(const Clipmap &) = delete;
    static std::unique_ptr<Clipmap> Create(const char *fileName, const Vector2i &center);
    ~Clipmap();
    void Update(const Vector2i &newCenter);

  private:
    Clipmap(File &&file, const Vector2i &center);
    File _file;

    int _width;
    int _height;
    int _nChannel;
    int _bitDepth;
    int _nLevel;

    int _clipWindowSize;
    Vector2i _clipCenter{-1, -1};

    std::vector<size_t> _mipmapOffset;
    std::vector<ClipMipmap> _clipStack;

    std::vector<uint8_t> ReadRegion(const Rect2i& region, int mipLevel);
};

} // namespace terrain

#endif