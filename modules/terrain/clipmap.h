#ifndef CLIPMAP_H
#define CLIPMAP_H

#include "core/math/vector2i.h"
#include "core/string/ustring.h"
#include "core/templates/rid.h"
#include <stdint.h>
#include <vector>

namespace terrain {

class Clipmap {
public:
    Clipmap(const String& mapPath, int nLevel, const Vector2i& center);
    void Update(const Vector2i& newCenter);
private:
    Vector2i _clipSize;
    Vector2i _clipCenter;

    std::vector<Vector<uint8_t>> _datas;
    std::vector<RID> _textureRIDs;
};

}

#endif