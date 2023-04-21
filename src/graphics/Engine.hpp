#ifndef ENGINE_H_
#define ENGINE_H_

#include <string>
#include <SDL2/SDL.h>
#include <GL/glew.h>

class GraphicsEngine {
    public:
        GraphicsEngine(int width, int height, std::string const& name);
        ~GraphicsEngine();

        void start_frame() { begin_ticks = SDL_GetTicks(); }
        void end_frame() const;

    private:
        SDL_Window* window;
        SDL_GLContext context;
        int32_t begin_ticks = 0;
};

#endif // ENGINE_H_
