#include "Mapper_003.hpp"
#include <iostream>

bool Mapper_003::cpuMapRead(uint16_t addr, uint32_t &mapped_addr, uint8_t&)
{
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }
    return false;
}

bool Mapper_003::cpuMapWrite(uint16_t addr, uint32_t &mapped_addr, uint8_t data)
{
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        nCHRBankSelect = data & 0x03;
        mapped_addr = addr;
    }
    return false;
}

bool Mapper_003::ppuMapRead(uint16_t addr, uint32_t &mapped_addr)
{
    if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        mapped_addr = nCHRBankSelect * 0x2000 + addr;
        return true;
    }
    return false;
}

bool Mapper_003::ppuMapWrite(uint16_t addr, uint32_t &mapped_addr)
{
    return false;
}

void Mapper_003::reset() {
    nCHRBankSelect = 0;
}
