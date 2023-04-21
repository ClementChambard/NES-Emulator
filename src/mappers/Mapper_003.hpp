#ifndef MAPPER_003_H_
#define MAPPER_003_H_

#include "Mapper.hpp"

class Mapper_003 : public Mapper {
    public:
        Mapper_003(uint8_t a, uint8_t b) : Mapper(a,b) { reset(); }
        bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr, uint8_t& data) override;
        bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr, uint8_t data = 0) override;
        bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) override;
        bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) override;
        void reset() override;

    private:
        uint8_t nCHRBankSelect = 0x00;
};

#endif // MAPPER_003_H_
