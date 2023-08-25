#ifndef BIN_TEXTURE_DATA
#define BIN_TEXTURE_DATA

#include "core/math/rect2i.h"
#include "core/math/vector2i.h"
#include "core/templates/vector.h"
#include <stdint.h>

namespace terrain {

/*
 Wrapper for binary texture data
*/
class TextureCache {
public:
    TextureCache() = delete;
    TextureCache(int size);
    Vector<uint8_t> GetRegion(const Rect2i& region);
    void UpdateRegion(const Rect2i& dstRegion, Vector<uint8_t> data);
private:
    int _size;
    Vector<uint8_t> _data;
};

}


#endif