#ifndef HEAP_H
#define HEAP_H
#include <stdio.h>
#define ROZMIAR_STRONY 4096
#define ROZMIAR_PLOTKA 16
#define WZORZEC_PLOTKA '#'

struct menadzer_sterty {
    void *poczatek_sterty;
    size_t rozmiar_sterty;
    struct blok_sterty *pierwszy_blok_sterty;
};

struct blok_sterty {
    struct blok_sterty *poprzedni;
    struct blok_sterty *nastepny;
    size_t rozmiar;
    int czy_wolny;
    int suma_kontrolna;
};

enum pointer_type_t {
    pointer_null,
    pointer_heap_corrupted,
    pointer_control_block,
    pointer_inside_fences,
    pointer_inside_data_block,
    pointer_unallocated,
    pointer_valid
};

int heap_setup(void);
void heap_clean(void);
void* heap_malloc(size_t size);
void* heap_calloc(size_t number, size_t size);
void* heap_realloc(void* memblock, size_t size);
void heap_free(void* memblock);
size_t heap_get_largest_used_block_size(void);
enum pointer_type_t get_pointer_type(const void* pointer);
int heap_validate(void);

#endif