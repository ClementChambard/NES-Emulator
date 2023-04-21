#ifndef MAPPER_000_H_
#define MAPPER_000_H_

#include "Mapper.hpp"

class Mapper_000 : public Mapper {
    public:
        Mapper_000(uint8_t a, uint8_t b) : Mapper(a,b) { reset(); }
        bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr, uint8_t& data) override;
        bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr, uint8_t data = 0) override;
        bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) override;
        bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) override;
        void reset() override {}
};

#endif // MAPPER_000_H_
