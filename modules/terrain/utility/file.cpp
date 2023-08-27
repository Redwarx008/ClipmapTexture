#include "file.hpp"
#include "core/error/error_macros.h"
#include "core/io/file_access.h"
#include "core/templates/vector.h"
#include "servers/rendering_server.h"
#include <intsafe.h>
#include <stdint.h>
#include <stdio.h>

namespace terrain {

File::File() {}

File::File(File&& other)
{
  this->_f = other._f;
  other._f = nullptr;
}

File::~File()
{
  if (_f) {
    fclose(_f);
  }
}

bool
File::Open(const char* fileName, FileMode mode)
{
  const char* mod;
  switch (mode) {
    case ReadOnly:
      mod = "rb";
      break;
    case WriteOnly:
      mod = "wb";
      break;
    case ReadWrite:
      mod = "rb+";
      break;
    case Apend:
      mod = "ab";
      break;
    default:
      // unreachable
      break;
  }
  if (0 != fopen_s(&_f, fileName, mod)) {
    return false;
  }
  return true;
}

bool
File::Close()
{
  if (_f) {
    fclose(_f);
    return true;
  }
  return false;
}

std::vector<uint8_t>
File::Read(size_t length) const
{
  std::vector<uint8_t> data;
  if (length < 0) {
    return data;
  }

  data.resize(length);
  size_t count = fread_s(&data[0], data.size(), 1, length, _f);
  if (count < length) {
    data.resize(count);
  }
  return data;
}

// not implemented yet, I'm lazy.
size_t
File::Read(void* buffer, size_t length) const
{
  return 0;
}

uint8_t
File::Read8() const
{
  ERR_FAIL_COND_V(!_f, 0);
  uint8_t ret;
  if (0 == fread_s(&ret, 1, 1, 1, _f)) {
    ret = '\0';
  }
  return ret;
}

// little endian
uint16_t
File::Read16() const
{
  ERR_FAIL_COND_V(!_f, 0);
  uint8_t low = Read8();
  uint8_t high = Read8();
  uint16_t ret = (uint16_t)high << 8 | (uint16_t)low;
  return ret;
}
}