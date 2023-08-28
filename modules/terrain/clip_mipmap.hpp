#ifndef TEXTURE_CACHE_H
#define TEXTURE_CACHE_H

#include "core/math/rect2i.h"
#include "core/math/vector2i.h"
#include "core/templates/vector.h"
#include <shared_mutex>
#include <stdint.h>
#include <vector>

namespace terrain
{

/*
 Wrapper for binary texture data
*/
class ClipMipmap
{
  public:
    ClipMipmap() = delete;
    ClipMipmap(int size, int nChannel, int bitDepth);
    inline int GetSize() const;
    Vector<uint8_t> GetRegion(const Rect2i &region);
    void UpdateRegion(const Rect2i &dstRegion, const std::vector<uint8_t> &data);

  private:
    int _size;
    int _pixelSize;
    Vector2i _center;
    std::shared_mutex _mutex;
    std::vector<uint8_t> _data;
};

} // namespace terrain

#endif