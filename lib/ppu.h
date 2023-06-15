#ifndef PPU_H
#define PPU_H

#include "cartridge.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct AddrRegister
{
    uint8_t value[2];
    bool high_pointer;
} AddrRegister;

typedef struct PPU {
    // Registers
    uint8_t controller; // 0x2000
    uint8_t mask; // 0x2001
    uint8_t status; // 0x2002
    uint8_t oam_addr; // 0x2003
    uint8_t oam_data_reg; // 0x2004
    uint8_t scroll; // 0x2005
    AddrRegister address; // 0x2006
    uint8_t data; // 0x2007
    uint8_t oam_dma; // 0x4014
    
    // Temporary buffer to hold data read from memory
    uint8_t internal_data_buffer;
    
    // Memory
    uint8_t *chr_rom;
    uint8_t palette_table[32];
    uint8_t vram[2048];
    uint8_t oam_data[256];

    Mirroring mirroring;
    int cycles;
    int scanline;
} PPU;

#define SCANLINE_CYCLES 341
#define MAX_VISIBLE_SCANLINES 240
#define MAX_SCANLINES 262

PPU *ppu_new(uint8_t *chr_rom, Mirroring mirroring);
bool ppu_tick(PPU *ppu, int cycles);

/*
    CONTROLLER REGISTER BITS

    7  bit  0
    ---- ----
    VPHB SINN
    |||| ||||
    |||| ||++- Base nametable address
    |||| ||    (0 = $2000; 1 = $2400; 2 = $2800; 3 = $2C00)
    |||| |+--- VRAM address increment per CPU read/write of PPUDATA
    |||| |     (0: add 1, going across; 1: add 32, going down)
    |||| +---- Sprite pattern table address for 8x8 sprites
    ||||       (0: $0000; 1: $1000; ignored in 8x16 mode)
    |||+------ Background pattern table address (0: $0000; 1: $1000)
    ||+------- Sprite size (0: 8x8 pixels; 1: 8x16 pixels – see PPU OAM#Byte 1)
    |+-------- PPU master/slave select
    |          (0: read backdrop from EXT pins; 1: output color on EXT pins)
    +--------- Generate an NMI at the start of the
               vertical blanking interval (0: off; 1: on)
*/

#define NAMETABLE_ADDR_1        0b00000001
#define NAMETABLE_ADDR_2        0b00000010
#define VRAM_ADDR_INCREMENT     0b00000100
#define SPRITE_PATTERN_ADDR     0b00001000
#define BACKGROUND_PATTERN_ADDR 0b00010000
#define SPRITE_SIZE             0b00100000
#define MASTER_SLAVE            0b01000000
#define GENERATE_NMI            0b10000000

// Controller register functions
bool ppu_controller_bit_is_set(PPU *ppu, uint8_t flag);
void ppu_controller_bit_set(PPU *ppu, uint8_t flag);
void ppu_controller_bit_unset(PPU *ppu, uint8_t flag);
void ppu_controller_register_set(PPU *ppu, uint8_t value);

// Status register defines
#define NOTUSED1        0b00000001
#define NOTUSED2        0b00000010
#define NOTUSED3        0b00000100
#define NOTUSED4        0b00001000
#define NOTUSED5        0b00010000
#define SPRITE_OVERFLOW 0b00100000
#define SPRITE_ZERO_HIT 0b01000000
#define VBLANK_STARTED  0b10000000

// Status register functions
void ppu_status_bit_set(PPU *ppu, uint8_t flag);
void ppu_status_bit_unset(PPU *ppu, uint8_t flag);
bool ppu_statuts_bit_is_set(PPU *ppu, uint8_t flag);

// Memory functions
uint8_t ppu_mem_read(PPU *ppu);
void ppu_mem_write(PPU *ppu, uint8_t value);

// VRAM functions
void ppu_vram_addr_increment(PPU *ppu);
uint16_t ppu_mirror_vram_addr(PPU *ppu, uint16_t addr);

// Write to register functions
void ppu_write_to_controller(PPU *ppu, uint8_t value);
void ppu_write_to_mask(PPU *ppu, uint8_t value);
void ppu_write_to_oam_addr(PPU *ppu, uint8_t value);
void ppu_write_to_oam_data(PPU *ppu, uint8_t value);
void ppu_write_to_scroll(PPU *ppu, uint8_t value);
void ppu_write_to_ppu_addr(PPU *ppu, uint8_t value);
void ppu_write_to_ppu_data(PPU *ppu, uint8_t value);
void ppu_write_to_oam_dma(PPU *ppu, uint8_t value);

// AddrRegister functions
AddrRegister addrregister_new(void);
void addrregister_set(AddrRegister addr_reg, uint16_t value);
void addrregister_update(AddrRegister addr_reg, uint8_t value);
void addrregister_increment(AddrRegister addr_reg, uint8_t value);
void addrregister_reset_pointer(AddrRegister addr_reg);
uint16_t addrregister_get(AddrRegister addr_reg);

#endif
