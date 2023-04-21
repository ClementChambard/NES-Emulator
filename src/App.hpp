#ifndef APP_H_
#define APP_H_

#include "Bus.hpp"

#include <NSNES_config.h>
#include "graphics/Engine.hpp"
#include "graphics/SpriteFont.hpp"

class App {
    public:
        App(std::string const& rom);
        ~App() = default;

        void run();

#ifdef DEBUG_VIEW
        void DrawString(int x, int y, std::string s, glm::vec<4, uint8_t> col = {255,255,255,255});
        void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns);
        void DrawCpu(int x, int y);
        void DrawCode(int x, int y, int nLines, std::map<uint16_t, std::string> const& mapAsm);
#endif

    private:
        GraphicsEngine context;

        Bus nes;

        Shader shader;
        VertexBatch batch;

#ifdef DEBUG_VIEW
        SpriteFont text;
        bool emulationMode = false;
        uint8_t selectedPalette = 0x00;
#endif

};

#endif // APP_H_
