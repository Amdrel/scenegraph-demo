#include "shaders/shader.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>

namespace scenegraphdemo {
    const size_t SHADER_LOG_SIZE = 512;

    Shader::Shader(const char *vertexShaderSource, const char *fragmentShaderSource) {
        this->program = this->linkProgram(
            this->createShader(vertexShaderSource, GL_VERTEX_SHADER),
            this->createShader(fragmentShaderSource, GL_FRAGMENT_SHADER));
    }

    void Shader::use() {
        glUseProgram(this->program);
    }

    void Shader::setUniformInt(const char *uniform, int value) {
        GLint location = glGetUniformLocation(this->program, uniform);
        glUniform1i(location, value);
    }

    void Shader::setUniformMat4(const char *uniform, float *value) {
        GLint location = glGetUniformLocation(this->program, uniform);
        glUniformMatrix4fv(location, 1, GL_FALSE, value);
    }

    unsigned int Shader::createShader(const char *source, unsigned int type) {
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        int success;
        char report[SHADER_LOG_SIZE];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, SHADER_LOG_SIZE, nullptr, report);
            std::cerr << report << std::endl;
            std::cerr << source << std::endl;
            throw ShaderCompilationError();
        }

        return shader;
    }

    unsigned int Shader::linkProgram(unsigned int vertexShader, unsigned int fragmentShader) {
        unsigned int program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        int success;
        char report[SHADER_LOG_SIZE];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, SHADER_LOG_SIZE, nullptr, report);
            std::cerr << report << std::endl;
            throw ShaderLinkingError();
        }

        return program;
    }
}
