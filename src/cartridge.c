#include "../lib/cartridge.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

uint8_t const NES_TAG[TAG_LENGTH] = {0x4E, 0x45, 0x53, 0x1A};

ROM *get_rom(char *file_path) {
    // Checks if file is a '.nes' file
    char *file_extension = strrchr(file_path, '.');
    if (strcmp(file_extension, ".nes") != 0) {
        fprintf(stderr, "Error: specified file isn't '.nes' file.\n");
        return NULL;
    }

    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Something went wrong when trying to open the file.\n");
        fclose(file);
        return NULL;
    }

    // Checks if file has the proper NES signature
    uint8_t header[16];
    fread(header, sizeof(uint8_t), 16, file);
    if (!check_header(header)) {
        fprintf(stderr, "Error: file isn't a '.nes' file of supported iNES type.\n");
        fclose(file);
        return NULL;
    }
    
    ROM *rom = malloc(sizeof(ROM));
    rom->prg_rom_length = header[PRG_ROM_LENGTH_ADDR] * PRG_ROM_PAGE_SIZE;
    rom->chr_rom_length = header[CHR_ROM_LENGTH_ADDR] * CHR_ROM_PAGE_SIZE;
    uint8_t control_byte_1 = header[CONTROL_BYTE_1_ADDR];
    uint8_t control_byte_2 = header[CONTROL_BYTE_2_ADDR];
    //uint8_t prg_ram_length = header[PRG_RAM_LENGTH_ADDR]; IMPLEMENT LATER
    
    if ((control_byte_2 & 0b00001111) != 0) {
        fprintf(stderr, "Warning: file isn't in the iNES 1.0 format.\n");
        //free(rom);
        //fclose(file);
        //return NULL;
    }
    
    rom->mapper = (control_byte_2 & 0b11110000) | (control_byte_1 >> 4);

    // Determine mirroring type
    if ((control_byte_1 & 0b1000) != 0) {
        rom->mirroring = FourScreen;
    }
    else if ((control_byte_1 & 0b1) != 0) {
        rom->mirroring = Vertical;
    }
    else {
        rom->mirroring = Horizontal;
    }

    // Checks if trainer is present and skips it if it is
    bool trainer = control_byte_1 & 0b100;
    if (trainer) {
        fseek(file, TRAINER_LENGTH, SEEK_CUR);
    }

    // Reads PRG and CHR ROM into the ROM struct
    rom->prg_rom = malloc(sizeof(uint8_t) * rom->prg_rom_length);
    rom->chr_rom = malloc(sizeof(uint8_t) * rom->chr_rom_length);
    
    fread(rom->prg_rom, sizeof(uint8_t), rom->prg_rom_length, file);
    fread(rom->chr_rom, sizeof(uint8_t), rom->chr_rom_length, file);

    fclose(file);
    return rom;
}

bool check_header(uint8_t *header) {
    for (int i = 0; i < TAG_LENGTH; i++) {
        if (header[i] != NES_TAG[i]) {
            return false;
        }
    }
    return true;
}
