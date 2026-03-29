#ifndef FILE_READER_H
#define FILE_READER_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#define ROZMIAR_SEKTORA 512

struct lancuch_klastrow_t{
    uint16_t *klastry;
    size_t rozmiar;
};

struct boot_t{
    char jump[3];
    uint64_t oem_name;
    uint16_t bitow_na_sektor;
    uint8_t sektory_na_klaster;
    uint16_t zarezerwowane_sektory;
    uint8_t FAT_ile;
    uint16_t root_ile;
    uint16_t sektory_logiczne16;
    uint8_t rodzaj_pliku;
    uint16_t FAT_rozmiar;
    uint16_t SPT;
    uint16_t ile_naglowkow;
    uint32_t sectors_number_before_start;
    uint32_t sectors_number_file_system;
    uint8_t  dysk_id;
    uint8_t  niewykorzystane;
    uint8_t  ebs;
    uint32_t numer_seryjny;
    char VL[11];
    uint64_t file_system_type;
    char boot_code[448];
    uint16_t signature_value;
} __attribute__((__packed__));

struct disk_t{
    FILE* dysk;
} __attribute__((__packed__));

struct volume_t{
    struct disk_t *dysk;
    struct boot_t boot;
    void *FAT1;
    void *FAT2;
    uint32_t root_pierwszy_sektor;
    uint32_t root_ile_sektorow;
    void *root;
} __attribute__((__packed__));

struct bity_t{
    unsigned char bit_0:1;
    unsigned char bit_1:1;
    unsigned char bit_2:1;
    unsigned char bit_3:1;
    unsigned char bit_4:1;
    unsigned char bit_5:1;
    unsigned char bit_6:1;
    unsigned char bit_7:1;
};

struct dir_t{
    struct volume_t *wolumin;
    int32_t ile_wczytano;
} __attribute__((__packed__));

struct dir_entry_pelny_t{
    char nazwa[11];
    struct bity_t bity;
    uint8_t zarezerowowane;
    uint8_t czas_utworzenia_dziesietny;
    uint16_t czas_utworzenia;
    uint16_t data_utworzenia;
    uint16_t ostatnio_modyfikowany;
    uint16_t pierwszy_klaster;
    uint16_t ostatnio_modyfikowany_czas;
    uint16_t ostatnio_modyfikowany_data;
    uint16_t ostatni_klaster;
    uint32_t rozmiar_pliku;
} __attribute__((__packed__));

struct dir_entry_t{
    char name[13];
    uint32_t size;
    uint8_t is_archived: 1;
    uint8_t is_readonly: 1;
    uint8_t is_system: 1;
    uint8_t is_hidden: 1;
    uint8_t is_directory: 1;
    uint16_t first_cluster;
};

struct file_t{
    struct volume_t *wolumin;
    struct lancuch_klastrow_t *klastry_pliku;
    struct dir_t *katalog_plikow;
    struct dir_entry_t wejscie_pliku;
    int32_t offset;
} __attribute__((__packed__));

struct disk_t* disk_open_from_file(const char* volume_file_name);
int disk_read(struct disk_t* pdisk, int32_t first_sector, void* buffer, int32_t sectors_to_read);
int disk_close(struct disk_t* pdisk);

struct volume_t* fat_open(struct disk_t* pdisk, uint32_t first_sector);
int fat_close(struct volume_t* pvolume);

uint16_t read_fat12_entry(const void *buffer, size_t size, uint16_t cluster);
struct lancuch_klastrow_t *get_chain_fat12(const void * const buffer, size_t size, uint16_t first_cluster);
struct file_t* file_open(struct volume_t* pvolume, const char* file_name);
int file_close(struct file_t* stream);
size_t file_read(void *ptr, size_t size, size_t nmemb, struct file_t *stream);
int32_t file_seek(struct file_t* stream, int32_t offset, int whence);

struct dir_t* dir_open(struct volume_t* pvolume, const char* dir_path);
int dir_read(struct dir_t* pdir, struct dir_entry_t* pentry);
int dir_close(struct dir_t* pdir);

#endif//FILE_READER_H