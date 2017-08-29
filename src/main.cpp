#include "logging.hpp"
#include "nodes/node.hpp"
#include "nodes/perspective_camera.hpp"
#include "resources/image_resource.hpp"
#include "resources/raw_resource.hpp"
#include "resources/resource.hpp"
#include "shaders/shader.hpp"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <boost/filesystem.hpp>
#include <cstdlib>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <memory>
#include <sstream>

using namespace scenegraphdemo;

constexpr int WINDOW_WIDTH = 1920 / 2;
constexpr int WINDOW_HEIGHT = 1080 / 2;

// Queries SDL for recent errors and prints them to stderr if present. Any
// accumulated errors are removed after being printed.
void checkSDLError(int line = -1) {
    std::string error = SDL_GetError();
    if (error != "") {
        std::cerr << "SDL Error: " << error << std::endl;
        if (line >= 0) {
            std::cerr << "\nLine: " << line << std::endl;
        }
        SDL_ClearError();
    }
}

// Sets OpenGL attributes to enable features required by the game. For example,
// this function will set attributes related to the OpenGL version and how to
// handle double buffering.
void setOpenGLAttributes() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

// Destroys the OpenGL context and cleans up SDL objects.
void cleanup(SDL_GLContext context, SDL_Window *window) {
    scenegraphdemo::info("Cleaning up resources used by SDL and OpenGL");
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Handle logic for the majority of SDL events and dispatch accordingly.
bool handleEvents(SDL_Event *event) {
    switch (event->type) {
    case SDL_QUIT:
        return false;
    case SDL_KEYDOWN:
        break;
    case SDL_KEYUP:
        break;
    }

    return true;
}

// The main game loop on the OpenGL thread. Draw calls are done here and at the
// moment input is processed here as well.
void run(SDL_Window *window) {
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    auto resourceDirStr = std::getenv("SCENEGRAPHDEMO_RESOURCE_DIR");
    if (resourceDirStr == nullptr) {
        scenegraphdemo::error("SCENEGRAPHDEMO_RESOURCE_DIR environment variable is undefined, so assets cannot be located");
        return;
    }
    boost::filesystem::path resourceDir = resourceDirStr;

    // Tightly-packed buffer containing vertex positions and texture coordinates
    // associated with them. These are used to draw a textured rectangle.
    const float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Load a texture to use for all the cubes.
    boost::filesystem::path textureTestPath = resourceDir / "textures/ground_03.jpg";
    ImageResource textureTest(textureTestPath.string());

    const float aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
    const float fov = glm::radians(45.0f);

    // Create a scenegraph with some cubes and a camera.
    auto scenegraph = std::make_shared<Node>("root");
    auto camera = std::make_shared<PerspectiveCamera>("camera",
        glm::vec3(0, 0, -5), glm::vec3(0, glm::radians(180.0f), 0), VEC3_ONE,
        fov, aspect, 0.1f, 100.0f);
    auto parentThing = std::make_shared<Node>("parentThing", glm::vec3(-2, 0, 0), VEC3_ZERO);
    auto childThing = std::make_shared<Node>("childThing", glm::vec3(2, 0, 0), VEC3_ZERO);
    scenegraph.get()->add(camera);
    scenegraph.get()->add(parentThing);
    parentThing.get()->add(childThing);
    scenegraph.get()->updateWorldTransform();

    // Compile a basic shader for use with the cube.
    boost::filesystem::path vertexShaderPath = resourceDir / "shaders/basic_vertex.glsl";
    RawResource vertexShader(vertexShaderPath.string());
    boost::filesystem::path fragmentShaderPath = resourceDir / "shaders/basic_fragment.glsl";
    RawResource fragmentShader(fragmentShaderPath.string());
    auto basicShader = Shader(vertexShader.data(), fragmentShader.data());
    basicShader.setUniformInt("texture0", 0);

    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = 0;
    float delta = 1.0f;
    float rotation = 0.0;

    bool running = true;
    while (running) {
        // Delta time is calculated then incorporated with time-related
        // functionality such as the rotation of the cubes. Delta time is used
        // so frame drops do not affect animation speeds.
        last = now;
        now = SDL_GetPerformanceCounter();
        delta = (double)((now - last) * 1000 / (double)SDL_GetPerformanceFrequency()) / 16 / 60;

        // Check for key / window system events (only used to check for close
        // request for this example).
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            running = handleEvents(&event);
        }

        // Update node transforms to demo scenegraph updates.
        rotation += glm::radians(180.0f) * delta; // 180 degrees a second.
        parentThing.get()->setRot(glm::vec3(0, rotation, 0));
        childThing.get()->setRot(glm::vec3(0, 0, rotation));
        camera.get()->setRot(glm::vec3(0, glm::radians(180.0f), rotation * 0.1));

        // Update the world transforms of nodes and their children if their
        // positions / rotations have been mutated.
        scenegraph.get()->updateWorldTransform();

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.3, 0.6, 0.8, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw the parent cube.
        auto modelViewProjectionMatrix = camera.get()->viewProjectionMatrix * parentThing.get()->worldTransform;
        basicShader.setUniformMat4("transform", glm::value_ptr(modelViewProjectionMatrix));
        basicShader.use();
        textureTest.bind();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // Draw the child cube.
        modelViewProjectionMatrix = camera.get()->viewProjectionMatrix * childThing.get()->worldTransform;
        basicShader.setUniformMat4("transform", glm::value_ptr(modelViewProjectionMatrix));
        basicShader.use();
        textureTest.bind();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        SDL_GL_SwapWindow(window);
    }
}

int main(int argc, char **argv) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL video subsystem (no display?)" << std::endl;
        checkSDLError(__LINE__);
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Scenegraph Demo", // Window title.
        SDL_WINDOWPOS_CENTERED, // Window position (X).
        SDL_WINDOWPOS_CENTERED, // Window position (Y).
        WINDOW_WIDTH, // Window width.
        WINDOW_HEIGHT, // Window height.
        SDL_WINDOW_OPENGL // SDL Flags.
    );
    if (window == nullptr) {
        std::cerr << "Failed to create window" << std::endl;
        checkSDLError(__LINE__);
        return 1;
    }

    setOpenGLAttributes();
    SDL_GL_SetSwapInterval(1);

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        std::cerr << "Unable to create OpenGL context (check your driver?)" << std::endl;
        checkSDLError(__LINE__);
        return 1;
    }
    glewExperimental = GL_TRUE;
    glewInit();

    scenegraphdemo::info("OpenGL context initialized");
    run(window); // OpenGL thread game loop.
    cleanup(context, window); // Cleanup resouces used by SDL and OpenGL.

    return 0;
}
