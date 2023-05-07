#include "../lib/bus.h"
#include "../lib/cartridge.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Bus *new_bus(ROM *rom)
{
    Bus *bus = malloc(sizeof(Bus));
    bus->rom = rom;
    memset(bus->ram, 0, sizeof(bus->ram));
    return bus;
}

uint8_t bus_mem_read(Bus *bus, uint16_t addr)
{
    // RAM
    if (addr >= RAM_START && addr <= RAM_MIRROR_END)
    {
        // RAM only takes into account the first 11 bits
        // This takes care of mirroring
        addr &= 0b11111111111;
        return bus->ram[addr];
    }
    // PRG ROM
    if (addr >= PRG_ROM_START && addr <= PRG_ROM_MIRROR_END)
    {
        addr -= PRG_ROM_START;
        // Mirror if needed
        if (bus->rom->prg_rom_length == 0x4000)
        {
            addr %= 0x4000;
        }
        return bus->rom->prg_rom[addr];
    }

    // TODO: PPU
    
    return 0;
}

void bus_mem_write(Bus *bus, uint8_t value, uint16_t addr)
{
    // RAM
    if (addr >= RAM_START && addr <= RAM_MIRROR_END)
    {
        // RAM only takes into account the first 11 bits
        // This takes care of mirroring
        addr &= 0b11111111111;
        bus->ram[addr] = value;
        return;
    }
    // PRG ROM
    else if (addr >= PRG_ROM_START && addr <= PRG_ROM_MIRROR_END)
    {
        fprintf(stderr, "Error: attempted to write to PRG ROM space.\n");
        return;
    }

    // TODO: PPU

    return;
}