#include "../lib/ppu.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

// Instantiates a new PPU
PPU *ppu_new(uint8_t *chr_rom, Mirroring mirroring)
{
    PPU *ppu = malloc(sizeof(PPU));
    // Initialize registers
    ppu->controller = 0;
    ppu->mask = 0;
    ppu->status = 0;
    ppu->oam_addr = 0;
    ppu->oam_data_reg = 0;
    ppu->scroll = 0;
    ppu->address = addrregister_new();
    ppu->data = 0;
    ppu->oam_dma = 0;

    ppu->chr_rom = chr_rom;
    ppu->mirroring = mirroring;


    memset(ppu->vram, 0, sizeof(ppu->vram)/sizeof(uint8_t));
    memset(ppu->vram, 0, sizeof(ppu->oam_data)/sizeof(uint8_t));
    memset(ppu->vram, 0, sizeof(ppu->palette_table)/sizeof(uint8_t));

    return ppu;
}

// Controller register functions

// Checks if a certain flag in the controller register is set
bool ppu_controller_bit_is_set(PPU *ppu, uint8_t flag)
{
    return ppu->controller & flag ? true : false;
}

// Sets the specified flag in the controller register
void ppu_controller_bit_set(PPU *ppu, uint8_t flag)
{
    ppu->controller |= flag;
}

// Unsets the specified flag in the controller register
void ppu_controller_bit_unset(PPU *ppu, uint8_t flag)
{
    ppu->controller &= ~flag;
}

// VRAM functions

// Increments the address register 
void ppu_vram_addr_increment(PPU *ppu)
{
    if (ppu_controller_bit_is_set(ppu, VRAM_ADDR_INCREMENT))
    {
        return addrregister_increment(ppu->address, 1);
    }
    else 
    {
        return addrregister_increment(ppu->address, 32);
    }
}

// Returns address to be accessed in the VRAM based on type of mirroring
uint16_t ppu_mirror_vram_addr(PPU *ppu, uint16_t addr)
{
    uint16_t mirrored_vram = addr & 0x2EFF;
    // Converts to index usable in the VRAM array
    uint16_t vram_index = mirrored_vram - 0x2000;
    uint16_t name_table = vram_index / 0x400;

    switch (ppu->mirroring)
    {
        case Vertical:
            return vram_index - 0x800;
        case Horizontal:
            if (name_table == 1 || name_table == 2)
            {
                return vram_index - 0x400;
            }
            else if (name_table == 3)
            {
                return vram_index - 0x800;
            }
        default:
            return vram_index;
    }
}

// Memory functions

// Read memory from PPU memory
uint8_t ppu_mem_read(PPU *ppu)
{
    uint16_t addr = addrregister_get(ppu->address);
    ppu_vram_addr_increment(ppu);
    
    // Read from CHR ROM
    if (addr <= 0x1FFF)
    {
        uint8_t result = ppu->internal_data_buffer;
        ppu->internal_data_buffer = ppu->chr_rom[addr];
        return result;
    }
    // Read from VRAM
    else if (addr >= 0x2000 && addr <= 0x2FFF) 
    {
        uint8_t result = ppu->internal_data_buffer;
        ppu->internal_data_buffer = ppu->vram[ppu_mirror_vram_addr(ppu, addr)];
        return result;
    }
    // Access to forbidden memory space
    else if (addr >= 0x3000 && addr <= 0x3EFF)
    {
        fprintf(stderr, "Access to forbidden memory space at %04X", addr);
        return 0;
    }
    // Read from palettes
    else if (addr >= 0x3F00 && addr <= 0x3FFF)
    {
        return ppu->palette_table[addr - 0x3F00];
    }
    else 
    {
        fprintf(stderr, "Unexpected access to mirrored space at %04X", addr);
        return 0;
    }
}

// Write to register functions

// Writes value to PPU controller register
void ppu_write_to_controller(PPU *ppu, uint8_t value)
{
    ppu->controller = value;
}

// Writes value to PPU mask register
void ppu_write_to_mask(PPU *ppu, uint8_t value)
{
    ppu->mask = value;
}

// Writes value to PPU OAM address register
void ppu_write_to_oam_addr(PPU *ppu, uint8_t value)
{
    ppu->oam_addr = value;
}

// Writes value to PPU OAM data register
void ppu_write_to_oam_data(PPU *ppu, uint8_t value)
{
    ppu->oam_data_reg = value;
}

// Writes value to PPU scroll register
void ppu_write_to_scroll(PPU *ppu, uint8_t value)
{
    ppu->scroll = value;
}

// Writes value to PPU address register
void ppu_write_to_ppu_addr(PPU *ppu, uint8_t value)
{
    addrregister_update(ppu->address, value);
}

// Writes value to PPU data register
void ppu_write_to_ppu_data(PPU *ppu, uint8_t value)
{
    ppu->data = value;
}

// Writes value to PPU OAM DMA register
void ppu_write_to_oam_dma(PPU *ppu, uint8_t value)
{
    ppu->oam_dma = value;
}

// AddrRegister functions

// Instantiates new AddrRegister
AddrRegister addrregister_new(void)
{
    AddrRegister addr_reg;
    addr_reg.value[0] = 0;
    addr_reg.value[1] = 0;
    addr_reg.high_pointer = true;
    return addr_reg;
}

// Sets the PPU's address register to a new value
void addrregister_set(AddrRegister addr_reg, uint16_t value)
{
    addr_reg.value[0] = value >> 8;
    addr_reg.value[1] = value & 0xFF;
}

// Updates one byte of the PPU's address register
void addrregister_update(AddrRegister addr_reg, uint8_t value)
{
    if (addr_reg.high_pointer)
    {
        addr_reg.value[0] = value;
    }
    else 
    {
        addr_reg.value[1] = value;
    }
    // Mirrors down any addresses above 0x3FFF
    uint16_t new_value = addrregister_get(addr_reg);
    if (new_value > 0x3FFF)
    {
        addrregister_set(addr_reg, new_value & 0x3FFF);
    }
    addr_reg.high_pointer = !addr_reg.high_pointer;
}

// Increments the PPU's address register's value by value.
// Handles wrap-around.
void addrregister_increment(AddrRegister addr_reg, uint8_t value)
{
    uint8_t low = addr_reg.value[1];
    addr_reg.value[1] += value;
    if (low > addr_reg.value[1])
    {
        addr_reg.value[0]++;
    }
    // Mirrors down any addresses above 0x3FFF
    uint16_t new_value = addrregister_get(addr_reg);
    if (new_value > 0x3FFF)
    {
        addrregister_set(addr_reg, new_value & 0x3FFF);
    }
}

// Resets the PPU's address register's 'pointer' to its default value (true).
// As a result, the next byte written to it will represent the msb of the new address.
void addrregister_reset_pointer(AddrRegister addr_reg)
{
    addr_reg.high_pointer = true;
}

uint16_t addrregister_get(AddrRegister addr_reg)
{
    return (uint16_t) addr_reg.value[0] << 8 | addr_reg.value[1];
}
