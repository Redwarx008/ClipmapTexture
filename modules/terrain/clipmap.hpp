#ifndef CLIPMAP_H
#define CLIPMAP_H

#include "core/math/rect2i.h"
#include "core/math/vector2i.h"
#include "core/templates/rid.h"
#include <intsafe.h>
#include <memory>
#include <stdint.h>
#include <vector>

#include "constants/TerrainConstants.hpp"
#include "ClipLevel.hpp"
#include "TiledBitmap.hpp"
#include "utility/File.hpp"


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
    Clipmap(TiledBitmap &&tiledBitmap, const Vector2i &center);
    TiledBitmap _tiledBitmap;

    int _width;
    int _height;
    int _nChannel;
    int _bitDepth;
    int _nLevel;

    int _clipWindowSize;
    Vector2i _clipCenter{-1, -1};

    std::vector<size_t> _mipmapOffset;
    std::vector<ClipLevel> _clipStack;
};

} // namespace terrain

#endif