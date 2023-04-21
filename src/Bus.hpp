#ifndef BUS_H_
#define BUS_H_

#include <cstdint>
#include <array>
#include <memory>
#include "Cartridge.hpp"
#include "Cpu6502.hpp"
#include "Ppu2C02.hpp"

class Bus {
    public:
        Bus();
        ~Bus();

        // Devices
        Cpu6502 cpu;
        Ppu2C02 ppu;
        uint8_t controller[2] = {};
        std::array<uint8_t, 2048> cpuRam;
        std::shared_ptr<Cartridge> cart;

        // Read Write
        void cpuWrite(uint16_t addr, uint8_t data);
        uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

        // System interface
        void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
        void reset();
        void clock();

    private:
        uint32_t nSystemClockCounter = 0;
        uint8_t controller_state[2] = {};

        uint8_t dma_page = 0x00;
        uint8_t dma_addr = 0x00;
        uint8_t dma_data = 0x00;

        bool dma_transfer = false;
        bool dma_dummy = true;
};

#endif // BUS_H_
