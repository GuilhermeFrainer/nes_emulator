#include "../lib/bus.h"

#include <string.h>
#include <stdlib.h>

Bus *new_bus(void)
{
    Bus *bus = malloc(sizeof(Bus));
    memset(bus->ram, 0, sizeof(bus->ram));
    return bus;
}

uint8_t bus_mem_read(Bus *bus, uint16_t addr)
{
    if (addr >= RAM_START && addr <= RAM_MIRROR_END)
    {
        // RAM only takes into account the first 11 bits
        // This takes care of mirroring
        addr &= 0b11111111111;
        return bus->ram[addr];
    }
    
    // TODO: PPU
    
    return 0;
}

void bus_mem_write(Bus *bus, uint8_t value, uint16_t addr)
{
    if (addr >= RAM_START && addr <= RAM_MIRROR_END)
    {
        // RAM only takes into account the first 11 bits
        // This takes care of mirroring
        addr &= 0b11111111111;
        bus->ram[addr] = value;
    }

    // TODO: PPU

    return;
}