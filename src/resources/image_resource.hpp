#pragma once

#include "GL/glew.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "resources/resource.hpp"

namespace scenegraphdemo {
    class ImageResource : public Resource {
    public:
        ImageResource(std::string filename);
        virtual ~ImageResource();

        // Returns a pointer to file data stored in memory.
        void *data();

        // Binds the texture for use with OpenGL.
        void bind(int texture = 0);
    private:
        // ID refering to a texture being managed by OpenGL.
        unsigned int texture;

        // Color format the texture is using. This can either be RGB or RGBA.
        GLint format;

        // Contains raw pixel image data processed by SDL.
        SDL_Surface *surface;
    };
}
