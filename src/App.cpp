#include "App.hpp"

std::string hex(uint32_t n, uint8_t d) {
    std::string s(d, '0');
    for (int i = d-1; i >= 0; i--, n >>= 4)
        s[i] = "0123456789ABCDEF"[n & 0xF];
    return s;
}

App::App(std::string const& rom) : context{WINDOW_WIDTH, WINDOW_HEIGHT, "NS nes"},
    nes{}, shader{"shaders/vertex.glsl", "shaders/fragment.glsl"}, batch{}
#ifdef DEBUG_VIEW
    ,text{"font.fmt"}
#endif
{

#ifdef DEBUG_VIEW
    text.set_halign(SpriteFont::LEFT);
    text.set_valign(SpriteFont::TOP);
#endif

    nes.insertCartridge(std::make_shared<Cartridge>(rom));
    nes.cpu.reset();
}

void App::run() {
    bool running = true;
    SDL_Event event;
    int32_t begin_ticks;
    while (running) {

        context.start_frame();

        glClearColor(0.1f, 0.2f, 0.8f, 1.f);
        glClearDepth(1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //nes->controller[0] = 0x00;
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_x: nes.controller[0] &= 0x7F; break;
                        case SDLK_z: nes.controller[0] &= 0xBF; break;
                        case SDLK_a: nes.controller[0] &= 0xDF; break;
                        case SDLK_s: nes.controller[0] &= 0xEF; break;
                        case SDLK_UP: nes.controller[0] &= 0xF7; break;
                        case SDLK_DOWN: nes.controller[0] &= 0xFB; break;
                        case SDLK_LEFT: nes.controller[0] &= 0xFD; break;
                        case SDLK_RIGHT: nes.controller[0] &= 0xFE; break;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
#ifdef DEBUG_VIEW
                        case SDLK_SPACE:
                            emulationMode = !emulationMode;
                            break;
#endif
                        case SDLK_x: nes.controller[0] |= 0x80; break;
                        case SDLK_z: nes.controller[0] |= 0x40; break;
                        case SDLK_a: nes.controller[0] |= 0x20; break;
                        case SDLK_s: nes.controller[0] |= 0x10; break;
                        case SDLK_UP: nes.controller[0] |= 0x08; break;
                        case SDLK_DOWN: nes.controller[0] |= 0x04; break;
                        case SDLK_LEFT: nes.controller[0] |= 0x02; break;
                        case SDLK_RIGHT: nes.controller[0] |= 0x01; break;
#ifdef DEBUG_VIEW
                        case SDLK_c:
                            do { nes.clock(); } while (!nes.cpu.complete());
                            do { nes.clock(); } while (nes.cpu.complete());
                            break;
                        case SDLK_f:
                            do { nes.clock(); } while (!nes.ppu.frame_complete);
                            do { nes.clock(); } while (!nes.cpu.complete());
                            nes.ppu.frame_complete = false;
                            break;
                        case SDLK_r:
                            nes.cpu.reset();
                            break;
                        case SDLK_i:
                            nes.cpu.irq();
                            break;
                        case SDLK_n:
                            nes.cpu.nmi();
                            break;
                        case SDLK_p:
                            ++selectedPalette &= 0x07;
#endif
                        default: break;
                    } break;
            }
        }

#ifdef DEBUG_VIEW
        if (emulationMode)
#endif
        {
            do { nes.clock(); } while (!nes.ppu.frame_complete);
            nes.ppu.frame_complete = false;
        }

        // Draw
        shader.use();

#ifdef DEBUG_VIEW
        DrawCpu(900, 4);

        for (int i = 0; i < 16; i++) {
            std::string s = hex(i, 2) + ": (" + std::to_string(nes.ppu.pOAM[i*4+3]) + ", " + std::to_string(nes.ppu.pOAM[i*4+0]) + ")";
            s += " ID: " + hex(nes.ppu.pOAM[i*4+1], 2) + " AT: " + hex(nes.ppu.pOAM[i*4+2], 2);
            DrawString(900, 144+i*20, s);
        }

        float s = 128/640.f;

        batch.draw_rectangle(176/640.f+s*1.1, 100/480.f-2*s, 0, s*2*480/640.f, s*2);
        batch.finish_adding();
        batch.render(&nes.ppu.getNameTable(0, selectedPalette));
        batch.clear_vertices();
        batch.draw_rectangle(176/680.f+s*2.9, 100/480.f-2*s, 0, s*2*480/640.f, s*2);
        batch.finish_adding();
        batch.render(&nes.ppu.getNameTable(1, selectedPalette));
        batch.clear_vertices();

        batch.draw_rectangle(176/640.f+s*1.1, -108/480.f-2*s, 0, s*2*480/640.f, s*2);
        batch.finish_adding();
        batch.render(&nes.ppu.getPatternTable(0, selectedPalette));
        batch.clear_vertices();
        batch.draw_rectangle(176/680.f+s*2.9, -108/480.f-2*s, 0, s*2*480/640.f, s*2);
        batch.finish_adding();
        batch.render(&nes.ppu.getPatternTable(1, selectedPalette));
        batch.clear_vertices();

        text.renderAllText();

        float w = 240*2/256.f*480/680.f;
        batch.draw_rectangle(w/2-1,0,0,w,2);
#else
        batch.draw_rectangle(0, 0, 0, 2, 2);
#endif
        batch.finish_adding();
        batch.render(&nes.ppu.getScreen());
        batch.clear_vertices();

        shader.unuse();

        context.end_frame();
    }
}



#ifdef DEBUG_VIEW
glm::vec2 pos(int x, int y) {
    return {(float)(x-WINDOW_WIDTH/2.f)*2/(float)WINDOW_WIDTH, (float)(-y+WINDOW_HEIGHT/2.f)*2/(float)WINDOW_HEIGHT};
}

void App::DrawString(int x, int y, std::string s, glm::vec<4, uint8_t> col) { SpriteFont::last_created_instance->renderText(s, pos(x, y), {0.5,0.5}, col); }

void App::DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns) {
    int nRamX = x, nRamY = y;
    for (int row = 0; row < nRows; row++) {
        std::string sOffset = "$" + hex(nAddr, 4) + ":";
        for (int col = 0; col < nColumns; col++) {
            sOffset += " " + hex(nes.cpuRead(nAddr, true), 2);
            nAddr++;
        }
        DrawString(nRamX, nRamY, sOffset);
        nRamY += 20;
    }
}

void App::DrawCpu(int x, int y)
{
    DrawString(x , y , "STATUS:");
    DrawString(x  + 128, y, "N",  nes.cpu.status & Cpu6502::N ? glm::vec<4, uint8_t>{0,255,0,255} : glm::vec<4, uint8_t>{255,0,0,255});
    DrawString(x  + 160, y , "V", nes.cpu.status & Cpu6502::V ? glm::vec<4, uint8_t>{0,255,0,255} : glm::vec<4, uint8_t>{255,0,0,255});
    DrawString(x  + 192, y , "-", nes.cpu.status & Cpu6502::U ? glm::vec<4, uint8_t>{0,255,0,255} : glm::vec<4, uint8_t>{255,0,0,255});
    DrawString(x  + 224, y , "B", nes.cpu.status & Cpu6502::B ? glm::vec<4, uint8_t>{0,255,0,255} : glm::vec<4, uint8_t>{255,0,0,255});
    DrawString(x  + 256, y , "D", nes.cpu.status & Cpu6502::D ? glm::vec<4, uint8_t>{0,255,0,255} : glm::vec<4, uint8_t>{255,0,0,255});
    DrawString(x  + 288, y , "I", nes.cpu.status & Cpu6502::I ? glm::vec<4, uint8_t>{0,255,0,255} : glm::vec<4, uint8_t>{255,0,0,255});
    DrawString(x  + 320, y , "Z", nes.cpu.status & Cpu6502::Z ? glm::vec<4, uint8_t>{0,255,0,255} : glm::vec<4, uint8_t>{255,0,0,255});
    DrawString(x  + 352, y , "C", nes.cpu.status & Cpu6502::C ? glm::vec<4, uint8_t>{0,255,0,255} : glm::vec<4, uint8_t>{255,0,0,255});
    DrawString(x , y + 20, "PC: $" + hex(nes.cpu.pc, 4));
    DrawString(x , y + 40, "A: $" +  hex(nes.cpu.a, 2) + "  [" + std::to_string(nes.cpu.a) + "]");
    DrawString(x , y + 60, "X: $" +  hex(nes.cpu.x, 2) + "  [" + std::to_string(nes.cpu.x) + "]");
    DrawString(x , y + 80, "Y: $" +  hex(nes.cpu.y, 2) + "  [" + std::to_string(nes.cpu.y) + "]");
    DrawString(x , y + 100, "Stack P: $" + hex(nes.cpu.stkp, 4));
}

void App::DrawCode(int x, int y, int nLines, std::map<uint16_t, std::string> const& mapAsm)
{
    auto it_a = mapAsm.find(nes.cpu.pc);
    int nLineY = (nLines >> 1) * 20 + y;
    if (it_a != mapAsm.end())
    {
        DrawString(x, nLineY, (*it_a).second, {0,255,255,255});
        while (nLineY < (nLines * 20) + y)
        {
            nLineY += 20;
            if (++it_a != mapAsm.end())
            {
                DrawString(x, nLineY, (*it_a).second);
            }
        }
    }

    it_a = mapAsm.find(nes.cpu.pc);
    nLineY = (nLines >> 1) * 20 + y;
    if (it_a != mapAsm.end())
    {
        while (nLineY > y)
        {
            nLineY -= 20;
            if (--it_a != mapAsm.end())
            {
                DrawString(x, nLineY, (*it_a).second);
            }
        }
    }
}

#endif
