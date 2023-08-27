#include "texture_cache.hpp"

#include <stdint.h>

#include <mutex>
#include <shared_mutex>

#include "core/error/error_macros.h"
#include "core/templates/vector.h"

namespace terrain
{

TextureCache::TextureCache(int size, int nChannel, int bitDepth)
{
    _size = size;
    _pixelSize = nChannel * bitDepth / 8;
    _data.resize(size * size * _pixelSize);
}

int TextureCache::GetSize() const
{
    return _size;
}

Vector<uint8_t> TextureCache::GetRegion(const Rect2i &region)
{
    Vector<uint8_t> data;
    if (region.position.x < 0 || region.position.x + region.size.x > _size || region.position.y < 0 ||
        region.position.y + region.size.y > _size)
    {
        ERR_FAIL_V_MSG(data, "region is out of bound.");
    }

    data.resize(region.size.x * region.size.y * _pixelSize);

    uint8_t *dstData = data.ptrw();

    {
        std::shared_lock<std::shared_mutex> readLock{_mutex};

        for (int y = 0; y < region.size.y; ++y)
        {
            for (int x = 0; x < region.size.x; ++x)
            {
                int srcX = region.position.x + x;
                int srcY = region.position.y + y;

                const uint8_t *src = &_data[(srcX + srcY * _size) * _pixelSize];
                uint8_t *dst = &dstData[(x + y * region.size.x) * _pixelSize];
                for (int i = 0; i < _pixelSize; ++i)
                {
                    dst[i] = src[i];
                }
            }
        }
    }

    return data;
}

void TextureCache::UpdateRegion(const Rect2i &dstRegion, const std::vector<uint8_t> &data)
{
    if (dstRegion.position.x < 0 || dstRegion.position.x + dstRegion.size.x > _size || dstRegion.position.y < 0 ||
        dstRegion.position.y + dstRegion.size.y > _size)
    {
        ERR_FAIL_MSG("dstRegion is out of bound.");
    }

    std::lock_guard<std::shared_mutex> writeLock {_mutex};

    for (int y = 0; y < dstRegion.size.y; ++y)
    {
        for (int x = 0; x < dstRegion.size.x; ++x)
        {
            int dstX = dstRegion.position.x + x;
            int dstY = dstRegion.position.y + y;

            const uint8_t* src = &data[(x + y * dstRegion.size.x) * _pixelSize];
            uint8_t* dst = &_data[(dstX + dstY * _size) * _pixelSize];
            for (int i = 0; i < _pixelSize; ++i)
            {
                dst[i] = src[i];
            }
        }
    }
}
} // namespace terrain