#include "../lib/ppu.h"
#include "../lib/bus.h"
#include "../lib/io.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

// Instantiates a new PPU
PPU *ppu_new(uint8_t *chr_rom, Mirroring mirroring) {
    PPU *ppu = malloc(sizeof(PPU));
    // Initialize registers
    ppu->controller = 0;
    ppu->mask = 0;
    ppu->status = 0;
    ppu->oam_addr = 0;
    ppu->oam_data_reg = 0;
    ppu->scroll = 0;
    ppu->data = 0;
    ppu->oam_dma = 0;

    ppu->chr_rom = chr_rom;
    ppu->mirroring = mirroring;
    ppu->cycle = 0;
    ppu->scanline = 0;

    ppu->interrupt = None;
    
    memset(ppu->vram, 0, sizeof(ppu->vram)/sizeof(ppu->vram[0]));
    memset(ppu->vram, 0, sizeof(ppu->oam_data)/sizeof(ppu->oam_data[0]));
    memset(ppu->vram, 0, sizeof(ppu->palette_table)/sizeof(ppu->palette_table[0]));

    return ppu;
}

// Returns interrupt to be performed
Interrupt ppu_tick(PPU *ppu, int cycles) {
    Color color = {0, 0, 0};
    Interrupt interrupt = None;
    for (int i = 0; i < cycles; i++) {
        color.r = ppu->cycle % 255;
        color.b = ppu->scanline % 255;
        color.g--;

        ppu->cycle++;
        
        if (ppu->cycle >=0 && ppu->cycle <= FRAME_WIDTH && ppu->scanline >= 0 && ppu->scanline <= FRAME_HEIGHT) {
            draw_pixel(frame, ppu->cycle, ppu->scanline, color);
        }
        if (ppu->cycle == 341) {
            ppu->scanline++;
            ppu->cycle = 0;
        }
        if (ppu->scanline == MAX_SCANLINES) {
            ppu->scanline = 0;
            interrupt = NMI;
        }
    }
    return interrupt;
}

// Checks if the specified flag in the controller register is set
bool ppu_controller_bit_is_set(PPU *ppu, uint8_t flag) {
    return ppu->controller & flag ? true : false;
}

// Sets the specified flag in the controller register
void ppu_controller_bit_set(PPU *ppu, uint8_t flag) {
    ppu->controller |= flag;
}

// Unsets the specified flag in the controller register
void ppu_controller_bit_unset(PPU *ppu, uint8_t flag) {
    ppu->controller &= ~flag;
}

// Status regiter functions
void ppu_status_bit_set(PPU *ppu, uint8_t flag) {
    ppu->status |= flag;
}

void ppu_status_bit_unset(PPU *ppu, uint8_t flag) {
    ppu->status &= ~flag;
}

bool ppu_statuts_bit_is_set(PPU *ppu, uint8_t flag) {
    return ppu->status & flag ? true : false;
}

// VRAM functions

// Increments the address register 
void ppu_vram_addr_increment(PPU *ppu) {
}

// Returns address to be accessed in the VRAM based on type of mirroring
uint16_t ppu_mirror_vram_addr(PPU *ppu, uint16_t addr) {
    uint16_t mirrored_vram = addr & 0x2EFF;
    // Converts to index usable in the VRAM array
    uint16_t vram_index = mirrored_vram - 0x2000;
    uint16_t name_table = vram_index / 0x400;

    switch (ppu->mirroring) {
        case Vertical:
            return vram_index - 0x800;
        case Horizontal:
            if (name_table == 1 || name_table == 2) {
                return vram_index - 0x400;
            }
            else if (name_table == 3) {
                return vram_index - 0x800;
            }
        default:
            return vram_index;
    }
}

// Memory functions

// Read memory from PPU memory
uint8_t ppu_mem_read(PPU *ppu) {
    return 0;
}

// Write to register functions

// Writes value to PPU controller register
void ppu_write_to_controller(PPU *ppu, uint8_t value) {
    ppu->controller = value;
}
// Writes value to PPU mask register
void ppu_write_to_mask(PPU *ppu, uint8_t value) {
    ppu->mask = value;
}

// Writes value to PPU OAM address register
void ppu_write_to_oam_addr(PPU *ppu, uint8_t value) {
    ppu->oam_addr = value;
}

// Writes value to PPU OAM data register
void ppu_write_to_oam_data(PPU *ppu, uint8_t value) {
    ppu->oam_data_reg = value;
}

// Writes value to PPU scroll register
void ppu_write_to_scroll(PPU *ppu, uint8_t value) {
    ppu->scroll = value;
}

// Writes value to PPU address register
void ppu_write_to_ppu_addr(PPU *ppu, uint8_t value) {
}

// Writes value to PPU data register
void ppu_write_to_ppu_data(PPU *ppu, uint8_t value) {
    ppu->data = value;
}

// Writes value to PPU OAM DMA register
void ppu_write_to_oam_dma(PPU *ppu, uint8_t value) {
    ppu->oam_dma = value;
}
