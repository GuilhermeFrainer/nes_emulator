#include "../lib/bus.h"
#include "../lib/cartridge.h"
#include "../lib/ppu.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Bus *new_bus(ROM *rom) {
    Bus *bus = malloc(sizeof(Bus));
    bus->rom = rom;
    memset(bus->ram, 0, sizeof(bus->ram));
    bus->ppu = ppu_new(rom->chr_rom, rom->mirroring); 
    bus->cycles = 0;
    return bus;
}

Interrupt bus_tick(Bus *bus, int cycles) {
    bus->cycles += cycles;
    Interrupt return_value = ppu_tick(bus->ppu, cycles * 3); // Multiplies cycles by 3 because each CPU cycle is 3 PPU cycles
    return return_value;
}

uint8_t bus_mem_read(Bus *bus, uint16_t addr) {
    // RAM
    if (addr >= RAM_START && addr <= RAM_MIRROR_END) {
        // RAM only takes into account the first 11 bits
        // This takes care of mirroring
        addr &= 0b11111111111;
        return bus->ram[addr];
    }
    // PPU memory access
    else if (addr == 0x2000 || addr == 0x2001 || addr == 0x2003 || addr == 0x2005 || addr == 0x2006 || addr == 0x4014) {
        fprintf(stderr, "Tried to read write-only ppu register at %04X\n", addr);
        return 0;
    }
    // Read from the status register
    else if (addr == 0x2002) {
        // Reads only the 3 most significant bits. The other 5 come from the buffer
        uint8_t data = (bus->ppu->status & 0xE0) | (bus->ppu->internal_data_buffer & 0x1F);
        ppu_status_bit_unset(bus->ppu, VBLANK_STARTED); // Reading unsets the VBLANK flag
        //bus->ppu->addr_latch = true; // Also resets the pointer of the address register
        return data;
    }
    else if (addr == 0x2007) {
        return ppu_mem_read(bus->ppu);
    }
    // Access to area mirroring PPU registers
    else if (addr >= PPU_MIRROR_START && addr <= PPU_MIRROR_END) {
        uint16_t mirrored_down_addr = addr & 0b0010000000000111;
        return bus_mem_read(bus, mirrored_down_addr);   
    }
    // PRG ROM
    else if (addr >= PRG_ROM_START && addr <= PRG_ROM_MIRROR_END) {
        addr -= PRG_ROM_START;
        // Mirror if needed
        if (bus->rom->prg_rom_length == 0x4000) {
            addr %= 0x4000;
        }
        return bus->rom->prg_rom[addr];
    }
    else {
        fprintf(stderr, "Ignoring memory access at address %04X.\n", addr);
        return 0;
    }
}

void bus_mem_write(Bus *bus, uint8_t value, uint16_t addr) {
    // RAM
    if (addr >= RAM_START && addr <= RAM_MIRROR_END) {
        // RAM only takes into account the first 11 bits
        // This takes care of mirroring
        addr &= 0b11111111111;
        bus->ram[addr] = value;
        return;
    }
    
    // PPU
    else if (addr >= 0x2000 && addr <= 0x2007) {
        switch (addr) {
            case 0x2000:
                ppu_write_to_controller(bus->ppu, value);
                return;
            case 0x2001:
                ppu_write_to_mask(bus->ppu, value);
                return;
            case 0x2002:
                fprintf(stderr, "Attempted to write to read-only PPU status register.\n");
                return;
            case 0x2003:
                ppu_write_to_oam_addr(bus->ppu, value);
                return;
            case 0x2004:
                ppu_write_to_oam_data(bus->ppu, value);
                return;
            case 0x2005:
                ppu_write_to_scroll(bus->ppu, value);
                return;
            case 0x2006:
                ppu_write_to_ppu_addr(bus->ppu, value);
                return;
            case 0x2007:
                ppu_write_to_ppu_data(bus->ppu, value);
                    ppu_vram_addr_increment(bus->ppu); // Writing to the data register increments the addr register
                return;
            default:
                fprintf(stderr, "Tried to access unknown PPU register: %04X.\n", addr);
                return;
        }
    }
    // PPU mirror space
    else if (addr >= PPU_MIRROR_START && addr <= PPU_MIRROR_END) {
        uint16_t mirrored_down_addr = addr & 0x2007;
        bus_mem_write(bus, value, mirrored_down_addr);
        return;
    }
    else if (addr == 0x4014) {
        ppu_write_to_oam_dma(bus->ppu, value);
        return;
    }
    // PRG ROM
    else if (addr >= PRG_ROM_START && addr <= PRG_ROM_MIRROR_END) {
        fprintf(stderr, "Error: attempted to write to PRG ROM space at %04X.\n", addr);
        return;
    }
    else  {
        fprintf(stderr, "Attempted to write to non-handled address at %04X.\n", addr);
        return;
    }
}

Interrupt bus_poll_for_interrupt(Bus *bus) {
    return bus->ppu->interrupt;
}

void bus_clear_interrupt(Bus *bus) {
    bus->ppu->interrupt = None;
}
