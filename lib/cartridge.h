#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <stdint.h>
#include <stdbool.h>

// Header defines
#define TAG_LENGTH 4
#define PRG_ROM_LENGTH_ADDR 4
#define CHR_ROM_LENGTH_ADDR 5
#define CONTROL_BYTE_1_ADDR 6
#define CONTROL_BYTE_2_ADDR 7
#define PRG_RAM_LENGTH_ADDR 8

#define TRAINER_LENGTH 512

// PRG and CHR ROM length are informed by a single byte each in the header
// The byte informs the length in kB
// 16 kB for PRG and 8 kB for CHR
#define PRG_ROM_PAGE_SIZE 16384
#define CHR_ROM_PAGE_SIZE 8192


typedef enum Mirroring {
    Vertical,
    Horizontal,
    FourScreen,
} Mirroring;

typedef struct ROM {
    uint8_t *prg_rom;
    uint8_t *chr_rom;
    int prg_rom_length;
    int chr_rom_length;
    uint8_t mapper;
    Mirroring mirroring;
} ROM;

extern const uint8_t NES_TAG[TAG_LENGTH];

ROM *get_rom(char *file_path);
bool check_header(uint8_t *header);

#endif