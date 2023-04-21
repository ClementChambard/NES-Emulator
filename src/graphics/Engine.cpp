#include "Engine.hpp"
#include <iostream>

GraphicsEngine::GraphicsEngine(int width, int height, std::string const& name)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0)  { std::cout << "Error: failed to initialize SDL.\n"; exit(1); }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    if (window == nullptr) { std::cout << "Error: failed to create window.\n"; exit(1); }

    context = SDL_GL_CreateContext(window);
    if (context == nullptr) { std::cout << "Error: failed to create context.\n"; exit(1); }

    GLenum Error = glewInit();
    if (Error != GLEW_OK) { std::cout << "Error: failed to initialize glew.\n"; exit(1); }

    SDL_GL_SetSwapInterval(1);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);
}

GraphicsEngine::~GraphicsEngine()
{
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void GraphicsEngine::end_frame() const {
     SDL_GL_SwapWindow(window);

    int frameTicks = SDL_GetTicks() - begin_ticks;
    if (frameTicks < 1000 / 60)
        SDL_Delay(1000 / 60 - frameTicks);
}
