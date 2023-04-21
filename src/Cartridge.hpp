#ifndef CARTRIDGE_H_
#define CARTRIDGE_H_

#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include "mappers/Mapper.hpp"

class Cartridge {
    public:
        Cartridge(std::string const& sFileName);
        ~Cartridge();

        bool cpuRead(uint16_t addr, uint8_t& data);
        bool cpuWrite(uint16_t addr, uint8_t data);

        bool ppuRead(uint16_t addr, uint8_t& data);
        bool ppuWrite(uint16_t addr, uint8_t data);

        void reset();

        bool ImageValid() const { return imageValid; }

        MIRROR mirror() const { return pMapper->mirror() == HARDWARE ? hw_mirror : pMapper->mirror(); }

        std::shared_ptr<Mapper> GetMapper() const { return pMapper; }

    private:
        std::vector<uint8_t> vPRGMemory;
        std::vector<uint8_t> vCHRMemory;

        std::shared_ptr<Mapper> pMapper;

        bool imageValid = false;
        MIRROR hw_mirror = HORIZONTAL;

        uint8_t nMapperID = 0;
        uint8_t nPRGBanks = 0;
        uint8_t nCHRBanks = 0;
};

#endif // CARTRIDGE_H_
