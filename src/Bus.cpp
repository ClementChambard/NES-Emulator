#include "Bus.hpp"

Bus::Bus() {
    // Clear ram
    for (auto &i : cpuRam) i = 0x00;
    cpu.ConnectBus(this);
}

Bus::~Bus() {

}

void Bus::cpuWrite(uint16_t addr, uint8_t data) {
    if (cart->cpuWrite(addr, data))
    {

    }
    else if (addr >= 0x0000 && addr <= 0x1FFF)
        cpuRam[addr & 0x7FF] = data;
    else if (addr >= 0x2000 && addr <= 0x3FFF)
    {
        ppu.cpuWrite(addr & 0x0007, data);
    }
    else if (addr == 0x4014)
    {
        dma_page = data;
        dma_addr = 0x00;
        dma_transfer = true;
    }
    else if (addr >= 0x4016 && addr <= 0x4017)
	{
		controller_state[addr & 0x0001] = controller[addr & 0x0001];
	}
}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly) {
    uint8_t data = 0x00;

    if (cart->cpuRead(addr, data))
    {

    }
    else if (addr >= 0x0000 && addr <= 0x1FFF)
        data = cpuRam[addr & 0x7FF];
    else if (addr >= 0x2000 && addr <= 0x3FFF)
        data = ppu.cpuRead(addr & 0x0007, bReadOnly);
    else if (addr >= 0x4016 && addr <= 0x4017)
	{
		data = (controller_state[addr & 0x0001] & 0x80) > 0;
		controller_state[addr & 0x0001] <<= 1;
	}

    return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
    cart = cartridge;
    ppu.connectCartridge(cartridge);
}

void Bus::reset()
{
    cpu.reset();
    nSystemClockCounter = 0;
    if (cart.get()) cart->reset();
}

void Bus::clock()
{
    ppu.clock();
    if (nSystemClockCounter % 3 == 0) {
        if (!dma_transfer) cpu.clock();
        else if (dma_dummy) { if (nSystemClockCounter % 2 == 1) dma_dummy = false; }
        else if (nSystemClockCounter % 2 == 0) dma_data = cpuRead((uint16_t)dma_page << 8 | dma_addr);
        else {
            ppu.pOAM[dma_addr++] = dma_data;
            if (dma_addr == 0x00) { dma_transfer = false; dma_dummy = true; }
        }
    }
    if (ppu.nmi) { ppu.nmi = false; cpu.nmi(); }
    if (cart->GetMapper()->irqState()) cart->GetMapper()->irqClear(), cpu.irq();
    nSystemClockCounter++;
}
