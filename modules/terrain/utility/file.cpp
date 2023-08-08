#include "File.hpp"
#include "core/error/error_macros.h"
#include "core/io/file_access.h"
#include "core/templates/vector.h"
#include "servers/rendering_server.h"
#include <intsafe.h>
#include <stdint.h>
#include <stdio.h>

namespace terrain
{

#if defined(_WIN32)
extern "C" __declspec(dllimport) int __stdcall MultiByteToWideChar(unsigned int cp, unsigned long flags,
                                                                   const char *str, int cbmb, wchar_t *widestr,
                                                                   int cchwide);
#endif

File::File()
{
}

File::File(File &&other)
{
    this->_f = other._f;
    other._f = nullptr;
}

File::~File()
{
    if (_f)
    {
        fclose(_f);
    }
}

bool File::Open(const char *fileName, FileMode mode)
{
    const char *mod;
    switch (mode)
    {
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

#if defined(_WIN32)
    wchar_t wMode[64];
    wchar_t wFilename[1024];
    if (0 == MultiByteToWideChar(65001 /* UTF8 */, 0, fileName, -1, wFilename, sizeof(wFilename) / sizeof(*wFilename)))
        return 0;

    if (0 == MultiByteToWideChar(65001 /* UTF8 */, 0, mod, -1, wMode, sizeof(wMode) / sizeof(*wMode)))
        return 0;
#if defined(_MSC_VER) && _MSC_VER >= 1400
    if (0 != _wfopen_s(&_f, wFilename, wMode))
    {
        return false;
    }
#else
    _f = _wfopen(wFilename, wMode);
    if (_f == nullptr)
    {
        return false;
    }
#endif

#else
    _f = fopen(filename, mod);
    if (_f == nullptr)
    {
        return false;
    }
#endif

    return true;
}

bool File::IsOpen()
{
    return _f != nullptr;
}

bool File::Close()
{
    if (_f)
    {
        fclose(_f);
        return true;
    }
    return false;
}

bool File::Seek(int64_t offset, int origin)
{
    return _fseeki64(_f, offset, origin) == 0;
}

std::vector<uint8_t> File::Read(size_t length) const
{
    std::vector<uint8_t> data;
    if (length < 0)
    {
        return data;
    }

    data.resize(length);
    size_t count = fread_s(&data[0], data.size(), 1, length, _f);
    if (count < length)
    {
        data.resize(count);
    }
    return data;
}

// not implemented yet, I'm lazy.
size_t File::Read(void *buffer, size_t length) const
{
    return 0;
}

uint8_t File::Read8() const
{
    ERR_FAIL_COND_V(!_f, 0);
    uint8_t ret;
    if (0 == fread_s(&ret, 1, 1, 1, _f))
    {
        ret = '\0';
    }
    return ret;
}

// little endian
uint16_t File::Read16() const
{
    ERR_FAIL_COND_V(!_f, 0);
    uint8_t low = Read8();
    uint8_t high = Read8();
    uint16_t ret = (uint16_t)high << 8 | (uint16_t)low;
    return ret;
}
} // namespace terrain