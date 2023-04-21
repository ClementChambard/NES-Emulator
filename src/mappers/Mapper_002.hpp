#ifndef MAPPER_002_H_
#define MAPPER_002_H_

#include "Mapper.hpp"

class Mapper_002 : public Mapper {
    public:
        Mapper_002(uint8_t a, uint8_t b) : Mapper(a,b) { reset(); }
        bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr, uint8_t& data) override;
        bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr, uint8_t data = 0) override;
        bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) override;
        bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) override;
        void reset() override;

    private:
        uint8_t nPRGBankSelectLo = 0x00;
        uint8_t nPRGBankSelectHi = 0x00;
};

#endif // MAPPER_002_H_
