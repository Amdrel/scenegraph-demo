#include "resources/raw_resource.hpp"
#include <fstream>
#include <cstddef>

namespace scenegraphdemo {
    RawResource::RawResource(std::string filename) {
        this->filename = filename;

        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open() || file.eof() || file.fail()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }

        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        std::size_t realSize = fileSize;
        realSize++; // +1 for null terminator.
        this->contents.resize(realSize);
        file.seekg(0, std::ios::beg);
        file.read(this->contents.data(), fileSize);

        // Add null terminator so the data can be reinterpreted as a char array
        // and we don't need to worry about accidental overflow.
        this->contents[this->contents.size() - 1] = '\0';
    }

    RawResource::~RawResource() {
    }

    const char *RawResource::data() {
        return this->contents.data();
    }
}
