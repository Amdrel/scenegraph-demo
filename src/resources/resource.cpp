#include "resources/resource.hpp"

namespace scenegraphdemo {
    Resource::Resource() {
    }

    Resource::Resource(std::string filename) {
        this->filename = filename;
    }

    Resource::~Resource() {
    }
}
