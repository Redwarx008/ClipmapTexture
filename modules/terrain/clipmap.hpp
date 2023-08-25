#ifndef CLIPMAP_H
#define CLIPMAP_H

#include "core/math/vector2i.h"
#include "core/templates/rid.h"
#include <intsafe.h>
#include <stdint.h>
#include <vector>
#include <memory>

#include "constants/terrain_constants.hpp"
#include "texture_cache.hpp"

namespace terrain {

class Clipmap
{
public:
  Clipmap(const Clipmap&) = delete;
  Clipmap& operator= (const Clipmap&) = delete;
  static std::unique_ptr<Clipmap> Create(const char* fileName, int nLevel, const Vector2i& center);
  ~Clipmap();
  void Update(const Vector2i& newCenter);

private:
  Clipmap(FILE* file, int nLevel, const Vector2i& center);
  FILE* _file;

  int _clipWindowSize;
  Vector2i _clipCenter;

  std::vector<size_t> _mipmapOffset;
  std::vector<TextureCache> _clipStack;
};

} // namespace terrain

#endif