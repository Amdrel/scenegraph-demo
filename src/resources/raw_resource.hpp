#pragma once

#include "resources/resource.hpp"
#include <vector>

namespace scenegraphdemo {
    class RawResource : public Resource {
    public:
        RawResource(std::string filename);
        virtual ~RawResource();

        // Returns a pointer to file data stored in memory. This data is stored
        // in the contents vector in a contiguous fashion as guaranteed by the
        // C++ standard.
        const char *data();
    private:
        // Buffer containing the contents of a file referenced by filename.
        std::vector<char> contents;
    };
}
