#ifndef MAPPER_H_
#define MAPPER_H_

#include <cstdint>

enum MIRROR {
    HARDWARE,
    HORIZONTAL,
    VERTICAL,
    ONESCREEN_LO,
    ONESCREEN_HI
};

class Mapper {
    public:
        Mapper(uint8_t prgBanks, uint8_t chrBanks) : nPRGBanks(prgBanks), nCHRBanks(chrBanks) {}
        ~Mapper() = default;

        virtual bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr, uint8_t& data) = 0;
        virtual bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr, uint8_t data = 0) = 0;
        virtual bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) = 0;
        virtual bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) = 0;

        virtual void reset() {}

        virtual MIRROR mirror() { return HARDWARE; }

        virtual bool irqState() { return false; }
        virtual void irqClear() {}

        virtual void scanline() {}

    protected:
        uint8_t nPRGBanks = 0;
        uint8_t nCHRBanks = 0;
};

#endif // MAPPER_H_
