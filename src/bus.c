#include "../lib/bus.h"
#include "../lib/cartridge.h"
#include "../lib/ppu.h"

#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Bus *new_bus(ROM *rom)
{
    Bus *bus = malloc(sizeof(Bus));
    bus->rom = rom;
    memset(bus->ram, 0, sizeof(bus->ram));
    bus->ppu = new_ppu(rom->chr_rom, rom->mirroring); 
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
    // PPU memory access
    else if (addr == 0x2000 || addr == 0x2001 || addr == 0x2003 || addr == 0x2005 || addr == 0x2006 || addr == 0x4014)
    {
        fprintf(stderr, "Tried to read write-only ppu register at %04X\n", addr);
        return 0;
    }
    else if (addr == 0x2007)
    {
        return ppu_mem_read(bus->ppu);
    }
    // Access to mirrored area
    else if (addr >= PPU_MIRROR_START && addr <= PPU_MIRROR_END)
    {
        uint16_t mirrored_down_addr = addr & 0b0010000000000111;
        return bus_mem_read(bus, mirrored_down_addr);   
    }
    // PRG ROM
    else if (addr >= PRG_ROM_START && addr <= PRG_ROM_MIRROR_END)
    {
        addr -= PRG_ROM_START;
        // Mirror if needed
        if (bus->rom->prg_rom_length == 0x4000)
        {
            addr %= 0x4000;
        }
        return bus->rom->prg_rom[addr];
    }
    else
    {
        fprintf(stderr, "Ignoring memory access at address %04X\n", addr);
        return 0;
    }
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
