#include "Cartridge.hpp"
#include <fstream>
#include <iostream>
#include "mappers/Mapper_000.hpp"
#include "mappers/Mapper_001.hpp"
#include "mappers/Mapper_002.hpp"
#include "mappers/Mapper_003.hpp"

Cartridge::Cartridge(std::string const& sFileName)
{
    struct sHeader {
        char name[4];
        uint8_t prg_rom_chunks;
        uint8_t chr_rom_chunks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prg_ram_size;
        uint8_t tv_system1;
        uint8_t tv_system2;
        char unused[5];
    } header;

    std::ifstream ifs;
    ifs.open(sFileName, std::ifstream::binary);
    if (ifs.is_open())
    {
        ifs.read((char*)&header, sizeof(sHeader));

        if (header.mapper1 & 0x04) ifs.seekg(512, std::ios_base::cur);

        nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
        hw_mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

        uint8_t nFileType = 1;
        if ((header.mapper2 & 0x0C) == 0x08) nFileType = 2;

        if (nFileType == 0) {}

        if (nFileType == 1)
        {
            nPRGBanks = header.prg_rom_chunks;
            vPRGMemory.resize(nPRGBanks * 16384);
            ifs.read((char*)vPRGMemory.data(), vPRGMemory.size());

            nCHRBanks = header.chr_rom_chunks;
            if (nCHRBanks == 0) vCHRMemory.resize(8192);
            else vCHRMemory.resize(nCHRBanks * 8192);
            ifs.read((char*)vCHRMemory.data(), vCHRMemory.size());
        }

        if (nFileType == 2)
        {
            nPRGBanks = ((header.prg_ram_size & 0x07) << 8) | header.prg_rom_chunks;
            vPRGMemory.resize(nPRGBanks * 16384);
            ifs.read((char*)vPRGMemory.data(), vPRGMemory.size());

            nCHRBanks = ((header.prg_ram_size & 0x38) << 8) | header.chr_rom_chunks;
            vCHRMemory.resize(nCHRBanks * 8192);
            ifs.read((char*)vCHRMemory.data(), vCHRMemory.size());
        }

        switch (nMapperID) {
            case 0: pMapper = std::make_shared<Mapper_000>(nPRGBanks, nCHRBanks); break;
            case 1: pMapper = std::make_shared<Mapper_001>(nPRGBanks, nCHRBanks); break;
            case 2: pMapper = std::make_shared<Mapper_002>(nPRGBanks, nCHRBanks); break;
            case 3: pMapper = std::make_shared<Mapper_003>(nPRGBanks, nCHRBanks); break;
            default: pMapper = std::make_shared<Mapper_000>(nPRGBanks, nCHRBanks); std::cout << "Unsupported mapper " << (int)nMapperID << "\n"; break;
        }

        imageValid = true;
        ifs.close();
    } else { std::cout << "file open failed\n"; }
}

Cartridge::~Cartridge()
{

}

bool Cartridge::cpuRead(uint16_t addr, uint8_t& data) {
    uint32_t mapped_addr = 0;
    if (pMapper->cpuMapRead(addr, mapped_addr, data)) {
        if (mapped_addr != 0xFFFFFFFF)
            data = vPRGMemory[mapped_addr];
        return true;
    }
    return false;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;
    if (pMapper->cpuMapWrite(addr, mapped_addr, data)) {
        if (mapped_addr != 0xFFFFFFFF)
            vPRGMemory[mapped_addr] = data;
        return true;
    }
    return false;
}

bool Cartridge::ppuRead(uint16_t addr, uint8_t& data) {
    uint32_t mapped_addr = 0;
    if (pMapper->ppuMapRead(addr, mapped_addr)) {
        data = vCHRMemory[mapped_addr];
        return true;
    }
    return false;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;
    if (pMapper->ppuMapWrite(addr, mapped_addr)) {
        vCHRMemory[mapped_addr] = data;
        return true;
    }
    return false;
}

void Cartridge::reset() { if (pMapper.get()) pMapper->reset(); }
