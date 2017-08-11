#include "GL/glew.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "resources/image_resource.hpp"
#include <stdexcept>

namespace scenegraphdemo {
    ImageResource::ImageResource(std::string filename) {
        this->filename = filename;

        SDL_Surface *surface = IMG_Load(filename.c_str());
        if (!surface) {
            throw std::runtime_error("Unable to load image: " + filename);
        }
        this->surface = surface;

        GLint format;
        if (surface->format->BytesPerPixel == 4) {
            format = GL_RGBA;
        } else if (surface->format->BytesPerPixel == 3) {
            format = GL_RGB;
        } else {
            throw std::runtime_error("Unsupported color format for image: " + filename);
        }
        this->format = format;

        // Generate a texture id and load the texture into the GPU.
        glGenTextures(1, &this->texture);
        this->bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
        glTexImage2D(GL_TEXTURE_2D, 0, format, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    ImageResource::~ImageResource() {
        SDL_FreeSurface(this->surface);
        this->surface = nullptr;
    }

    void ImageResource::bind(int texture) {
        glActiveTexture(GL_TEXTURE0 + texture);
        glBindTexture(GL_TEXTURE_2D, this->texture);
    }

    void *ImageResource::data() {
        if (surface) {
            return this->surface->pixels;
        } else {
            return nullptr;
        }
    }
}
