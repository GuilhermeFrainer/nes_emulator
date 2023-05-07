#ifndef BUS_H
#define BUS_H

#include <stdint.h>

#define RAM_START 0x0000
#define RAM_MIRROR_END 0x1FFF

#define PPU_MEM_START 0x2000
#define PPU_MIRROR_END 0x3FFF

#define PRG_ROM_START 0x8000
#define PRG_ROM_MIRROR_END 0xFFFF

typedef struct ROM ROM;

typedef struct Bus {
    uint8_t ram[0x0800];
    ROM *rom;
} Bus;

Bus *new_bus(ROM *rom);
uint8_t bus_mem_read(Bus *bus, uint16_t addr);
void bus_mem_write(Bus *bus, uint8_t value, uint16_t addr);

#endif