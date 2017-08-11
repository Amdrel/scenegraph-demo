#pragma once

#include <glm/glm.hpp>
#include <string>

namespace scenegraphdemo {
    struct ShaderCompilationError : public std::exception {
        const char *what() const throw() {
            return "Unable to compile shader";
        }
    };

    struct ShaderLinkingError : public std::exception {
        const char *what() const throw() {
            return "Unable to link shader";
        }
    };

    // Wrapper around an OpenGL shader program with utility functions for
    // compilation, linking and usage of the shader program.
    class Shader {
    public:
        // TODO: Implemente a resource manager and switch the types.
        Shader(const char *vertexShaderSource, const char *fragmentShaderSource);

        // Sets the active OpenGL shader to this shader program. This needs to
        // be done before drawing an object each time one's rendered.
        void use();

        // Allows setting integer uniform values for the shader.
        void setUniformInt(const char *uniform, int value);

        // Allows setting mat4 uniforms for the shader.
        void setUniformMat4(const char *uniform, float *value);
    private:
        // OpenGL id for the shader program associated with the shader object.
        unsigned int program;

        // Compiles shader source of of a specified type and returns an OpenGL
        // id pointing to the unlinked shader. This function can throw a
        // ShaderCompilationError if the source code is malformed.
        unsigned int createShader(const char *source, unsigned int type);

        // Links a vertex and fragment shader into an OpenGL shader program.
        // This function will throw a ShaderLinkingError should it fail. This
        // function will also delete the shaders passed into it once linking is
        // finished (or has failed).
        unsigned int linkProgram(unsigned int vertexShader, unsigned int fragmentShader);
    };
}
