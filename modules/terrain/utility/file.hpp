#ifndef FILE_H
#define FILE_H

#include <intsafe.h>
#include <stdint.h>
#include <stdio.h>
#include <vector>

namespace terrain {

enum FileMode : uint8_t
{
    // Open a binary file for reading. (The file must exist.)
    ReadOnly,
    // Open an empty binary file for writing. 
    WriteOnly,
    // Open a binary file for both reading and writing. (The file must exist.)
    ReadWrite,
    // Open a text file in append mode for reading or updating at the end of the file.The file will be created if it does not exist.
    Apend,
};

class File
{
public:
    File();
    File(File&& other);
    ~File();
    bool Open(const char* fileName, FileMode mode);
    bool Close();
    std::vector<uint8_t> Read(size_t length) const;
    size_t Read(void* buffer, size_t length) const;

    uint8_t Read8() const;
    uint16_t Read16() const;
private:
    FILE* _f;
};

} // namespace terrain

#endif