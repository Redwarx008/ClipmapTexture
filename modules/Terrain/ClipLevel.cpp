#include "ClipLevel.hpp"

#include <stdint.h>

#include <mutex>
#include <shared_mutex>

#include "core/error/error_macros.h"
#include "core/templates/vector.h"

namespace terrain
{

ClipLevel::ClipLevel(int size, int nChannel, int bitDepth)
{
    _size = size;
    _pixelSize = nChannel * bitDepth / 8;
    
}

int ClipLevel::GetSize() const
{
    return _size;
}

Vector<uint8_t> ClipLevel::GetRegion(const Rect2i &region)
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

    }

    return data;
}

void ClipLevel::UpdateRegion(const Rect2i &dstRegion, const std::vector<uint8_t> &data)
{
    if (dstRegion.position.x < 0 || dstRegion.position.x + dstRegion.size.x > _size || dstRegion.position.y < 0 ||
        dstRegion.position.y + dstRegion.size.y > _size)
    {
        ERR_FAIL_MSG("dstRegion is out of bound.");
    }

    std::lock_guard<std::shared_mutex> writeLock{_mutex};


}
} // namespace terrain