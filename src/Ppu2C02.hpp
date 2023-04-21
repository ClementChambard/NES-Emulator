#ifndef PPU2C02_H_
#define PPU2C02_H_

#include <cstdint>
#include <memory>
#include "Cartridge.hpp"
#include "graphics/VertexBatch.hpp"
#include "graphics/PixelBuffer.hpp"

class Ppu2C02 {
    public:
        Ppu2C02();
        ~Ppu2C02();

        uint8_t cpuRead(uint16_t addr, bool rdonly = false);
        void cpuWrite(uint16_t addr, uint8_t data);

        uint8_t ppuRead(uint16_t addr, bool rdonly = false);
        void ppuWrite(uint16_t addr, uint8_t data);

        void connectCartridge(const std::shared_ptr<Cartridge>& cartridge) { cart = cartridge; }
        void clock();

        PixelBuffer& getScreen() { return sprScreen; }
        PixelBuffer& getNameTable(uint8_t i, uint8_t palette);
        PixelBuffer& getPatternTable(uint8_t i, uint8_t palette);

        Pixel& getColFromPalette(uint8_t pal, uint8_t pix);

        bool frame_complete = false;
        bool nmi = false;

        uint8_t tblName[2][1024] = {};
        uint8_t tblPalette[32] = {};
        uint8_t tblPattern[2][4096] = {};
    private:
        std::shared_ptr<Cartridge> cart;

        Pixel palScreen[0x40];
        PixelBuffer sprScreen{256,240};
        PixelBuffer sprNameTable[2] = {{256,240}, {256,240}};
        PixelBuffer sprPatternTable[2] = {{128,128}, {128,128}};

        int16_t scanline = 0;
        int16_t cycle = 0;

        union {
            struct {
                uint8_t unused : 5;
                uint8_t sprite_overflow : 1;
                uint8_t sprite_zero_hit : 1;
                uint8_t vertical_blank : 1;
            };
            uint8_t reg;
        } status;

        union {
            struct {
                uint8_t grayscale : 1;
                uint8_t render_background_left : 1;
                uint8_t render_sprites_left : 1;
                uint8_t render_background : 1;
                uint8_t render_sprites : 1;
                uint8_t enhance_red : 1;
                uint8_t enhance_green : 1;
                uint8_t enhance_blue : 1;
            };
            uint8_t reg;
        } mask;

        union PPUCTRL {
            struct {
                uint8_t nametable_x : 1;
                uint8_t nametable_y : 1;
                uint8_t increment_mode : 1;
                uint8_t pattern_sprite : 1;
                uint8_t pattern_background : 1;
                uint8_t sprite_size : 1;
                uint8_t slave_mode : 1;
                uint8_t enable_nmi : 1;
            };
            uint8_t reg;
        } control;

        union loopy_register
        {
                // Credit to Loopy for working this out :D
                struct
                {

                        uint16_t coarse_x : 5;
                        uint16_t coarse_y : 5;
                        uint16_t nametable_x : 1;
                        uint16_t nametable_y : 1;
                        uint16_t fine_y : 3;
                        uint16_t unused : 1;
                };

                uint16_t reg = 0x0000;
        };

        loopy_register vram_addr; // Active "pointer" address into nametable to extract background tile info
        loopy_register tram_addr; // Temporary store of information to be "transferred" into "pointer" at various times

        uint8_t address_latch = 0x00;
        uint8_t ppu_data_buffer = 0x00;

        uint8_t fine_x = 0x00;

        uint8_t bg_next_tile_id = 0x00;
        uint8_t bg_next_tile_attrib = 0x00;
        uint8_t bg_next_tile_msb = 0x00;
        uint8_t bg_next_tile_lsb = 0x00;

        uint16_t bg_shifter_pattern_lo = 0x0000;
        uint16_t bg_shifter_pattern_hi = 0x0000;
        uint16_t bg_shifter_attrib_lo = 0x0000;
        uint16_t bg_shifter_attrib_hi = 0x0000;

        struct ObjectAttributeEntry {
            uint8_t y;
            uint8_t id;
            uint8_t attribute;
            uint8_t x;
        } OAM[64];

        ObjectAttributeEntry spriteScanline[8];
        uint8_t sprite_count;
        uint8_t sprite_shifter_pattern_lo[8];
        uint8_t sprite_shifter_pattern_hi[8];

        bool spriteZeroHitPossible = false;
        bool spriteZeroBeingRendered = false;

    public:
        uint8_t* pOAM = (uint8_t*)OAM;
        uint8_t oam_addr = 0x00;
};

#endif // PPU2C02_H_
