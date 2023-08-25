
#include "clipmap.hpp"

#include <corecrt_wstdio.h>
#include <memory>
#include <stdio.h>
#include <utility>

#include "core/error/error_macros.h"
#include "core/io/file_access.h"
#include "core/variant/variant.h"
#include "servers/rendering/rendering_device.h"
#include "servers/rendering_server.h"

#include "constants/terrain_constants.hpp"

namespace terrain {

Clipmap::~Clipmap()
{
  fclose(_file);
}

std::unique_ptr<Clipmap>
Clipmap::Create(const char* fileName, int nLevel, const Vector2i& center)
{
  FILE* file;
  if (0 != fopen_s(&file, fileName, "rb")) {
    ERR_FAIL_V_MSG(nullptr, vformat("can't open file %s", fileName));
  }

  auto ret = std::unique_ptr<Clipmap>(new Clipmap(file, nLevel, center));
  return std::move(ret);
}

Clipmap::Clipmap(FILE* file, int nLevel, const Vector2i& center)
{
  // RenderingDevice* rd =
  // RenderingServer::get_singleton()->get_rendering_device();
  _file = file;
  _clipCenter = center;

  _clipWindowSize = CLIP_WINDOW_SIZE;

  for(int i = 0; i < nLevel; ++i)
  {
    _clipStack.emplace_back(TextureCache(CLIP_CACHE_SIZE));
  }
}

} // namespace terrain
