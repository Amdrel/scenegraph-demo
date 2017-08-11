#pragma once

#include <string>

namespace scenegraphdemo {
    class Resource {
    public:
        Resource();
        Resource(std::string filename);
        virtual ~Resource();
    protected:
        // Path to the file that pertains to the data held by this object.
        std::string filename;
    };
}
