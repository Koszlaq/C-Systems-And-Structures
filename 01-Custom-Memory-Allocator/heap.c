#include "heap.h"
#include <stdint.h>
#include <string.h>
#include "custom_unistd.h"

struct menadzer_sterty sterta;

int heap_setup(void) {
    sterta.poczatek_sterty=custom_sbrk(ROZMIAR_STRONY);
    sterta.rozmiar_sterty=ROZMIAR_STRONY;
    sterta.pierwszy_blok_sterty=NULL;

    return 0;
}

void heap_clean(void) {
    heap_validate();
    custom_sbrk(-((uint8_t*)custom_sbrk(0)-(uint8_t*)sterta.poczatek_sterty));
    memset(&sterta,0,sizeof(struct menadzer_sterty));
}

int oblicz_sume_kontrolna(struct blok_sterty *blok) {
    uint8_t *dane=(uint8_t*)blok;
    int suma=0;

    for (int i=0;i<2*ROZMIAR_PLOTKA;i++)
        suma=suma+dane[i];

    return suma;
}

int niewykorzystany_rozmiar(void* blok) {
    struct blok_sterty *obecny_blok=(struct blok_sterty*)((uint8_t*)blok-3*ROZMIAR_PLOTKA);
    int nie_uzyte=0;

    if(obecny_blok->nastepny!=NULL) {
        uint8_t *ptr=(uint8_t*)obecny_blok+4*ROZMIAR_PLOTKA+obecny_blok->rozmiar;
        while(ptr<(uint8_t*)obecny_blok->nastepny){
            nie_uzyte++;
            ptr++;
        }
    }

    return nie_uzyte;
}

void* heap_malloc(size_t size) {
    if(size==0)
        return NULL;

    if (size+3*ROZMIAR_PLOTKA > sterta.rozmiar_sterty) {
        size_t ile_potrzebnej_pamieci=size+4*ROZMIAR_PLOTKA-sterta.rozmiar_sterty+ROZMIAR_STRONY-1&~(ROZMIAR_STRONY-1);
        if(custom_sbrk(ile_potrzebnej_pamieci)==(void*)-1)
            return NULL;

        sterta.rozmiar_sterty=ile_potrzebnej_pamieci+sterta.rozmiar_sterty;
    }

    struct blok_sterty *pierwszy_blok=sterta.pierwszy_blok_sterty;
    if(pierwszy_blok==NULL) {
        if(size+3*ROZMIAR_PLOTKA <= sterta.rozmiar_sterty) {
            pierwszy_blok=(struct blok_sterty *)sterta.poczatek_sterty;
            memset((uint8_t*)sterta.poczatek_sterty+2*ROZMIAR_PLOTKA,WZORZEC_PLOTKA, ROZMIAR_PLOTKA);
            memset((uint8_t*)sterta.poczatek_sterty+3*ROZMIAR_PLOTKA+size,WZORZEC_PLOTKA, ROZMIAR_PLOTKA);
            pierwszy_blok->poprzedni=NULL;
            pierwszy_blok->nastepny=NULL;
            pierwszy_blok->czy_wolny=0;
            pierwszy_blok->rozmiar=size;
            pierwszy_blok->suma_kontrolna=0;
            pierwszy_blok->suma_kontrolna=oblicz_sume_kontrolna(pierwszy_blok);

            sterta.rozmiar_sterty=sterta.rozmiar_sterty+(-size-4*ROZMIAR_PLOTKA);
            sterta.pierwszy_blok_sterty=pierwszy_blok;
        }
    }
    else if(size+4*ROZMIAR_PLOTKA <= sterta.rozmiar_sterty) {
        size_t offset=0;
        struct blok_sterty *wolny_blok=sterta.poczatek_sterty;

        while(wolny_blok->czy_wolny==0 || wolny_blok->czy_wolny==1) {
            if (wolny_blok->czy_wolny==1 && wolny_blok->rozmiar>=size+2*ROZMIAR_PLOTKA)
                break;

            if(wolny_blok->czy_wolny==0) {
                offset=offset+2*ROZMIAR_PLOTKA;
                offset=offset+niewykorzystany_rozmiar((uint8_t*)wolny_blok+3*ROZMIAR_PLOTKA);
            }
            offset=offset+wolny_blok->rozmiar+2*ROZMIAR_PLOTKA;

            if(wolny_blok->nastepny!=NULL)
                wolny_blok=wolny_blok->nastepny;
            else
                break;
        }

        if((uint8_t *)sterta.poczatek_sterty+offset+4*ROZMIAR_PLOTKA+size >= (uint8_t*)custom_sbrk(0))
            return NULL;

        if (wolny_blok->czy_wolny==0) {
            struct blok_sterty* nastepny_blok=(struct blok_sterty*) ((uint8_t*)wolny_blok+4*ROZMIAR_PLOTKA+wolny_blok->rozmiar);
            if((uint8_t*)nastepny_blok+2*ROZMIAR_PLOTKA+size >= (uint8_t*)custom_sbrk(0))
                return NULL;

            memset((uint8_t*)nastepny_blok+2*ROZMIAR_PLOTKA,WZORZEC_PLOTKA,ROZMIAR_PLOTKA);
            memset((uint8_t*)nastepny_blok+3*ROZMIAR_PLOTKA+size,WZORZEC_PLOTKA,ROZMIAR_PLOTKA);
            nastepny_blok->poprzedni=wolny_blok;
            nastepny_blok->nastepny=NULL;
            nastepny_blok->czy_wolny=0;
            nastepny_blok->rozmiar=size;
            nastepny_blok->suma_kontrolna=0;
            nastepny_blok->suma_kontrolna=oblicz_sume_kontrolna(nastepny_blok);

            wolny_blok->nastepny=nastepny_blok;
            wolny_blok->suma_kontrolna=0;
            wolny_blok->suma_kontrolna=oblicz_sume_kontrolna(wolny_blok);

            sterta.rozmiar_sterty=sterta.rozmiar_sterty+(-size-4*ROZMIAR_PLOTKA);

            if(get_pointer_type((uint8_t*)nastepny_blok+3*ROZMIAR_PLOTKA) == pointer_valid)
                return (uint8_t*)nastepny_blok+3*ROZMIAR_PLOTKA;

            return NULL;
        }
        if(wolny_blok->czy_wolny==1) {
            memset((uint8_t*)wolny_blok+2*ROZMIAR_PLOTKA,WZORZEC_PLOTKA,ROZMIAR_PLOTKA);
            memset((uint8_t*)wolny_blok+3*ROZMIAR_PLOTKA+size,WZORZEC_PLOTKA,ROZMIAR_PLOTKA);
            wolny_blok->czy_wolny=0;
            wolny_blok->rozmiar=size;
            wolny_blok->suma_kontrolna=0;
            wolny_blok->suma_kontrolna=oblicz_sume_kontrolna(wolny_blok);

            sterta.rozmiar_sterty=sterta.rozmiar_sterty+(-size-4*ROZMIAR_PLOTKA);
            if(get_pointer_type((uint8_t*)wolny_blok+3*ROZMIAR_PLOTKA) == pointer_valid)
                return (uint8_t*)wolny_blok+3*ROZMIAR_PLOTKA;

            return NULL;
        }
    }


    if(get_pointer_type((uint8_t*)pierwszy_blok+3*ROZMIAR_PLOTKA) == pointer_valid)
        return (uint8_t*)pierwszy_blok+3*ROZMIAR_PLOTKA;

    return NULL;
}

void heap_free(void* memblock) {
    if(memblock==NULL||get_pointer_type(memblock)!=pointer_valid)
        return;

    unsigned long wolny_rozmiar=0, prawdziwy_rozmiar=0, nie_uzyty=0;
    struct blok_sterty *obecny_blok=(struct blok_sterty*)((uint8_t*)memblock-3*ROZMIAR_PLOTKA);
    if(obecny_blok->czy_wolny==1)
        return;

    memset((uint8_t*)obecny_blok+2*ROZMIAR_PLOTKA,WZORZEC_PLOTKA,ROZMIAR_PLOTKA);
    memset((uint8_t*)obecny_blok+3*ROZMIAR_PLOTKA+obecny_blok->rozmiar,WZORZEC_PLOTKA,ROZMIAR_PLOTKA);
    obecny_blok->czy_wolny=1;
    wolny_rozmiar=obecny_blok->rozmiar+2*ROZMIAR_PLOTKA;

    if(obecny_blok->nastepny!=NULL) {
        uint8_t *ptr=(uint8_t*)obecny_blok+4*ROZMIAR_PLOTKA+obecny_blok->rozmiar;

        while(ptr<(uint8_t*)obecny_blok->nastepny){
            nie_uzyty++;
            ptr++;
        }

        obecny_blok->rozmiar=obecny_blok->rozmiar+nie_uzyty;
        wolny_rozmiar=wolny_rozmiar+nie_uzyty;
    }
    prawdziwy_rozmiar=wolny_rozmiar;

    if(obecny_blok->poprzedni!=NULL&&obecny_blok->nastepny!=NULL && obecny_blok->poprzedni->czy_wolny==1&&obecny_blok->nastepny->czy_wolny==1){
        wolny_rozmiar=wolny_rozmiar+4*ROZMIAR_PLOTKA;
        prawdziwy_rozmiar=prawdziwy_rozmiar+4*ROZMIAR_PLOTKA+obecny_blok->poprzedni->rozmiar+obecny_blok->nastepny->rozmiar;
        if (obecny_blok->nastepny->nastepny==NULL && obecny_blok->poprzedni->poprzedni!=NULL){
            wolny_rozmiar=wolny_rozmiar+2*ROZMIAR_PLOTKA;
            obecny_blok->poprzedni->poprzedni->nastepny=NULL;
            obecny_blok->poprzedni->poprzedni->suma_kontrolna=0;
            obecny_blok->poprzedni->poprzedni->suma_kontrolna=oblicz_sume_kontrolna(obecny_blok->poprzedni->poprzedni);

            sterta.rozmiar_sterty=sterta.rozmiar_sterty+wolny_rozmiar;

            return;
        }

        obecny_blok->poprzedni->rozmiar=prawdziwy_rozmiar;
        obecny_blok->poprzedni->nastepny=obecny_blok->nastepny->nastepny;

        if(obecny_blok->nastepny->nastepny!=NULL) {
            obecny_blok->nastepny->nastepny->poprzedni=obecny_blok->poprzedni;
            obecny_blok->nastepny->nastepny->suma_kontrolna=0;
            obecny_blok->nastepny->nastepny->suma_kontrolna=oblicz_sume_kontrolna(obecny_blok->nastepny->nastepny);
        }

        obecny_blok->poprzedni->suma_kontrolna=0;
        obecny_blok->poprzedni->suma_kontrolna=oblicz_sume_kontrolna(obecny_blok->poprzedni);
    }
    else if(obecny_blok->poprzedni!=NULL && obecny_blok->poprzedni->czy_wolny==1) {
        wolny_rozmiar=wolny_rozmiar+2*ROZMIAR_PLOTKA;
        prawdziwy_rozmiar=prawdziwy_rozmiar+2*ROZMIAR_PLOTKA+obecny_blok->poprzedni->rozmiar;

        if(obecny_blok->nastepny==NULL && obecny_blok->poprzedni->poprzedni!=NULL) {
            wolny_rozmiar=wolny_rozmiar+2*ROZMIAR_PLOTKA;
            obecny_blok->poprzedni->poprzedni->nastepny=NULL;
            obecny_blok->poprzedni->poprzedni->suma_kontrolna=0;
            obecny_blok->poprzedni->poprzedni->suma_kontrolna=oblicz_sume_kontrolna(obecny_blok->poprzedni->poprzedni);

            sterta.rozmiar_sterty=sterta.rozmiar_sterty+wolny_rozmiar;

            return;
        }

        obecny_blok->poprzedni->rozmiar=prawdziwy_rozmiar;
        obecny_blok->poprzedni->nastepny=obecny_blok->nastepny;

        if(obecny_blok->nastepny!=NULL) {
            obecny_blok->nastepny->poprzedni=obecny_blok->poprzedni;
            obecny_blok->nastepny->suma_kontrolna=0;
            obecny_blok->nastepny->suma_kontrolna=oblicz_sume_kontrolna(obecny_blok->nastepny);
        }

        obecny_blok->poprzedni->suma_kontrolna=0;
        obecny_blok->poprzedni->suma_kontrolna=oblicz_sume_kontrolna(obecny_blok->poprzedni);
    }
    else if(obecny_blok->nastepny!=NULL && obecny_blok->nastepny->czy_wolny==1) {
        wolny_rozmiar=wolny_rozmiar+2*ROZMIAR_PLOTKA;
        prawdziwy_rozmiar=prawdziwy_rozmiar+2*ROZMIAR_PLOTKA+obecny_blok->nastepny->rozmiar;

        if(obecny_blok->nastepny->nastepny==NULL && obecny_blok->poprzedni!=NULL) {
            wolny_rozmiar += 2*ROZMIAR_PLOTKA;
            obecny_blok->poprzedni->nastepny=NULL;
            obecny_blok->poprzedni->suma_kontrolna=0;
            obecny_blok->poprzedni->suma_kontrolna=oblicz_sume_kontrolna(obecny_blok->poprzedni);

            sterta.rozmiar_sterty=sterta.rozmiar_sterty+wolny_rozmiar;

            return;
        }

        obecny_blok->rozmiar=prawdziwy_rozmiar;

        if(obecny_blok->nastepny->nastepny!=NULL) {
            obecny_blok->nastepny->nastepny->poprzedni=obecny_blok;
            obecny_blok->nastepny->nastepny->suma_kontrolna=0;
            obecny_blok->nastepny->nastepny->suma_kontrolna=oblicz_sume_kontrolna(obecny_blok->nastepny->nastepny);
        }

        obecny_blok->nastepny=obecny_blok->nastepny->nastepny;
    }
    if(obecny_blok->nastepny==NULL&&obecny_blok->poprzedni!=NULL && obecny_blok->poprzedni->czy_wolny==0) {
        wolny_rozmiar=wolny_rozmiar+2*ROZMIAR_PLOTKA;
        obecny_blok->poprzedni->nastepny=NULL;
        obecny_blok->poprzedni->suma_kontrolna=0;
        obecny_blok->poprzedni->suma_kontrolna=oblicz_sume_kontrolna(obecny_blok->poprzedni);
    }

    obecny_blok->rozmiar=prawdziwy_rozmiar;

    int ile_wolnych_blokow=0, ktory_blok_w_stercie=0;
    struct blok_sterty *ptr=sterta.pierwszy_blok_sterty;
    while (ptr!=NULL){
        if(ptr->czy_wolny==1)
            ile_wolnych_blokow++;
        ktory_blok_w_stercie++;
        ptr=ptr->nastepny;
    }

    if(ile_wolnych_blokow==ktory_blok_w_stercie) {
        wolny_rozmiar=wolny_rozmiar+2*ROZMIAR_PLOTKA;
        sterta.pierwszy_blok_sterty=NULL;
    }

    obecny_blok->suma_kontrolna=0;
    obecny_blok->suma_kontrolna=oblicz_sume_kontrolna(obecny_blok);

    sterta.rozmiar_sterty=sterta.rozmiar_sterty+wolny_rozmiar-nie_uzyty;
}

void* heap_calloc(size_t number, size_t size) {
    void* ptr=heap_malloc(number*size);

    if (ptr!=NULL)
        memset(ptr,0,number*size);

    return ptr;
}

void* heap_realloc(void* memblock, size_t size) {
    if(sterta.poczatek_sterty==NULL)
        return NULL;

    if(size==0){
        heap_free(memblock);
        return NULL;
    }

    if(get_pointer_type(memblock)==pointer_valid || get_pointer_type(memblock)==pointer_null) {
        if (memblock==NULL)
            return (uint8_t *) heap_malloc(size);

        struct blok_sterty *obecny_blok=(struct blok_sterty *) ((uint8_t *)memblock-3*ROZMIAR_PLOTKA);
        if (size==obecny_blok->rozmiar)
            return (uint8_t *)memblock;

        if (size<obecny_blok->rozmiar) {
            sterta.rozmiar_sterty=sterta.rozmiar_sterty+(obecny_blok->rozmiar-size);

            memset((uint8_t*)obecny_blok+3*ROZMIAR_PLOTKA+obecny_blok->rozmiar,WZORZEC_PLOTKA,ROZMIAR_PLOTKA);
            memset((uint8_t*)obecny_blok+3*ROZMIAR_PLOTKA+size,WZORZEC_PLOTKA,ROZMIAR_PLOTKA);
            obecny_blok->rozmiar=size;
            obecny_blok->suma_kontrolna=0;
            obecny_blok->suma_kontrolna=oblicz_sume_kontrolna(obecny_blok);

            if(get_pointer_type(memblock)==pointer_valid)
                return (uint8_t*)memblock;
        }

        if (size>obecny_blok->rozmiar) {
            unsigned int ile_do_sbrk=0;
            size_t size2=0;
            uint8_t *ptr_obecny_blok=(uint8_t*)obecny_blok;

            if(obecny_blok->nastepny==NULL && ptr_obecny_blok+obecny_blok->rozmiar+4*ROZMIAR_PLOTKA<(uint8_t*)custom_sbrk(0)) {
                while (ptr_obecny_blok+obecny_blok->rozmiar+4*ROZMIAR_PLOTKA != (uint8_t *)custom_sbrk(0)) {
                    ile_do_sbrk++;
                    ptr_obecny_blok++;
                }
            }

             if(obecny_blok->nastepny!=NULL && obecny_blok->nastepny->czy_wolny==1 && obecny_blok->rozmiar+obecny_blok->nastepny->rozmiar+niewykorzystany_rozmiar(memblock)+2*ROZMIAR_PLOTKA>=size) {
                sterta.rozmiar_sterty=sterta.rozmiar_sterty+2*ROZMIAR_PLOTKA;
                memset((uint8_t*)obecny_blok+3*ROZMIAR_PLOTKA+obecny_blok->rozmiar,WZORZEC_PLOTKA,ROZMIAR_PLOTKA);
                obecny_blok->rozmiar=size;
                obecny_blok->nastepny->nastepny->poprzedni=obecny_blok;
                obecny_blok->nastepny=obecny_blok->nastepny->nastepny;
                memset((uint8_t*)obecny_blok+3*ROZMIAR_PLOTKA+obecny_blok->rozmiar,WZORZEC_PLOTKA,ROZMIAR_PLOTKA);

                obecny_blok->suma_kontrolna=0;
                obecny_blok->suma_kontrolna= oblicz_sume_kontrolna(obecny_blok);
                obecny_blok->nastepny->suma_kontrolna=0;
                obecny_blok->nastepny->suma_kontrolna=oblicz_sume_kontrolna(obecny_blok->nastepny);

                size2=size-obecny_blok->rozmiar;
                sterta.rozmiar_sterty=sterta.rozmiar_sterty-size2;

                if(get_pointer_type((uint8_t*)obecny_blok+3*ROZMIAR_PLOTKA)==pointer_valid)
                    return (uint8_t*)obecny_blok+3*ROZMIAR_PLOTKA;
            }

            if(obecny_blok->nastepny==NULL && obecny_blok->rozmiar+ile_do_sbrk>=size) {
                if((uint8_t*)obecny_blok+4*ROZMIAR_PLOTKA+size >= (uint8_t*)custom_sbrk(0))
                    return NULL;

                memset((uint8_t*)obecny_blok+3*ROZMIAR_PLOTKA+obecny_blok->rozmiar,WZORZEC_PLOTKA,ROZMIAR_PLOTKA);
                obecny_blok->rozmiar=size;
                memset((uint8_t*)obecny_blok+size+3*ROZMIAR_PLOTKA,WZORZEC_PLOTKA,ROZMIAR_PLOTKA);

                obecny_blok->suma_kontrolna=0;
                obecny_blok->suma_kontrolna=oblicz_sume_kontrolna(obecny_blok);

                size2=size-obecny_blok->rozmiar;
                sterta.rozmiar_sterty=sterta.rozmiar_sterty-size2;

                if(get_pointer_type(memblock)==pointer_valid)
                    return (uint8_t*)memblock;
            }

            if(obecny_blok->nastepny==NULL && obecny_blok->rozmiar+ile_do_sbrk<size) {
                size_t wymagany_rozmiar=size-obecny_blok->rozmiar-ile_do_sbrk+ROZMIAR_STRONY-1&~(ROZMIAR_STRONY-1);
                if (custom_sbrk(wymagany_rozmiar)==(void *)-1)
                    return NULL;

                sterta.rozmiar_sterty=sterta.rozmiar_sterty+wymagany_rozmiar;
                if((uint8_t*)obecny_blok+4*ROZMIAR_PLOTKA+size >= (uint8_t*)custom_sbrk(0))
                    return NULL;

                memset((uint8_t*)obecny_blok+3*ROZMIAR_PLOTKA+obecny_blok->rozmiar,WZORZEC_PLOTKA,ROZMIAR_PLOTKA);
                obecny_blok->rozmiar=size;
                memset((uint8_t*)obecny_blok+size+3*ROZMIAR_PLOTKA,WZORZEC_PLOTKA,ROZMIAR_PLOTKA);

                obecny_blok->suma_kontrolna=0;
                obecny_blok->suma_kontrolna=oblicz_sume_kontrolna(obecny_blok);

                size2=size-obecny_blok->rozmiar;
                sterta.rozmiar_sterty=sterta.rozmiar_sterty-size2;

                if(get_pointer_type(memblock)==pointer_valid)
                    return (uint8_t*)memblock;
            }

            if(obecny_blok->nastepny!=NULL) {
                struct blok_sterty *wolny_blok=(struct blok_sterty *)(uint8_t *)sterta.poczatek_sterty;
                while (wolny_blok!=NULL) {
                    if (wolny_blok->czy_wolny==1 && wolny_blok->rozmiar>=size+2*ROZMIAR_PLOTKA) {
                        memcpy((uint8_t*)wolny_blok+2*ROZMIAR_PLOTKA,(uint8_t*)obecny_blok+2*ROZMIAR_PLOTKA,obecny_blok->rozmiar+ROZMIAR_PLOTKA);
                        memset((uint8_t*)wolny_blok+size+3*ROZMIAR_PLOTKA,WZORZEC_PLOTKA,ROZMIAR_PLOTKA);
                        wolny_blok->rozmiar=size;
                        wolny_blok->czy_wolny=0;

                        wolny_blok->suma_kontrolna=0;
                        wolny_blok->suma_kontrolna=oblicz_sume_kontrolna(wolny_blok);

                        heap_free((uint8_t*)obecny_blok+3*ROZMIAR_PLOTKA);

                        sterta.rozmiar_sterty =sterta.rozmiar_sterty+(-size-2*ROZMIAR_PLOTKA);
                        if (get_pointer_type((uint8_t *)wolny_blok+3*ROZMIAR_PLOTKA) == pointer_valid)
                            return (uint8_t *)wolny_blok+3*ROZMIAR_PLOTKA;
                    }
                    wolny_blok=wolny_blok->nastepny;
                }

                size_t wymagany_rozmiar=size + 4*ROZMIAR_PLOTKA+ROZMIAR_STRONY-1&~(ROZMIAR_STRONY-1);
                if (custom_sbrk(wymagany_rozmiar)==(void *)-1)
                    return NULL;
                sterta.rozmiar_sterty=sterta.rozmiar_sterty+wymagany_rozmiar;

                struct blok_sterty *pozostaly_blok=obecny_blok;
                while (pozostaly_blok!=NULL && pozostaly_blok->nastepny!=NULL)
                    pozostaly_blok=pozostaly_blok->nastepny;


                if((uint8_t*)pozostaly_blok+8*ROZMIAR_PLOTKA+pozostaly_blok->rozmiar+niewykorzystany_rozmiar((uint8_t*)pozostaly_blok+3*ROZMIAR_PLOTKA)+size >= (uint8_t*)custom_sbrk(0))
                    return NULL;

                struct blok_sterty *nowy_blok=(struct blok_sterty *)((uint8_t *) pozostaly_blok+pozostaly_blok->rozmiar+4*ROZMIAR_PLOTKA+niewykorzystany_rozmiar((uint8_t*)pozostaly_blok+3*ROZMIAR_PLOTKA));
                memcpy((uint8_t *)nowy_blok+2*ROZMIAR_PLOTKA,(uint8_t *)obecny_blok+2*ROZMIAR_PLOTKA,ROZMIAR_PLOTKA+obecny_blok->rozmiar);
                nowy_blok->czy_wolny=0;
                nowy_blok->rozmiar=size;
                nowy_blok->poprzedni=pozostaly_blok;
                nowy_blok->nastepny=NULL;
                pozostaly_blok->nastepny=nowy_blok;
                memset((uint8_t *)nowy_blok+nowy_blok->rozmiar+3*ROZMIAR_PLOTKA,WZORZEC_PLOTKA,ROZMIAR_PLOTKA);

                nowy_blok->suma_kontrolna=0;
                nowy_blok->suma_kontrolna=oblicz_sume_kontrolna(nowy_blok);
                pozostaly_blok->suma_kontrolna=0;
                pozostaly_blok->suma_kontrolna=oblicz_sume_kontrolna(pozostaly_blok);


                heap_free((uint8_t*)obecny_blok+3*ROZMIAR_PLOTKA);
                sterta.rozmiar_sterty=sterta.rozmiar_sterty-(size-4*ROZMIAR_PLOTKA);

                if (get_pointer_type((uint8_t *)nowy_blok+3*ROZMIAR_PLOTKA) == pointer_valid)
                    return (uint8_t *)nowy_blok+3*ROZMIAR_PLOTKA;
            }
        }
    }
    return NULL;
}

size_t heap_get_largest_used_block_size(void) {
    if (heap_validate()==1)
        return 0;

    size_t najwiekszy_blok=0;
    struct blok_sterty *obecny_blok=sterta.pierwszy_blok_sterty;

    while (obecny_blok!=NULL) {
        if (obecny_blok->czy_wolny==0)
            if (obecny_blok->rozmiar>najwiekszy_blok)
                najwiekszy_blok=obecny_blok->rozmiar;
        obecny_blok=obecny_blok->nastepny;
    }
    return najwiekszy_blok;
}

enum pointer_type_t get_pointer_type(const void *const pointer) {
    if (pointer==NULL)
        return pointer_null;

    struct blok_sterty *obecny_blok=sterta.pierwszy_blok_sterty;
    while (obecny_blok!=NULL) {
        if ((void *) ((char *)obecny_blok+sizeof(struct blok_sterty)+ROZMIAR_PLOTKA)==pointer) {
            if (obecny_blok->czy_wolny==1)
                return pointer_unallocated;
            return pointer_valid;
        }

        if ((void *) ((char *)obecny_blok)<=pointer && (void *) ((char *)obecny_blok+sizeof(struct blok_sterty)+obecny_blok->rozmiar+2*ROZMIAR_PLOTKA)>=pointer) {
            if (obecny_blok->czy_wolny==1)
                return pointer_unallocated;
            if ((void *) ((char *)obecny_blok+sizeof(struct blok_sterty))>pointer)
                return pointer_control_block;
            if ((void *) ((char *)obecny_blok+sizeof(struct blok_sterty)+ROZMIAR_PLOTKA)>=pointer)
                return pointer_inside_fences;
            if ((void *) ((char *)obecny_blok+sizeof(struct blok_sterty)+ROZMIAR_PLOTKA+obecny_blok->rozmiar)>pointer)
                return pointer_inside_data_block;
            if ((void *) ((char *)obecny_blok+sizeof(struct blok_sterty)+2*ROZMIAR_PLOTKA+obecny_blok->rozmiar)>pointer)
                return pointer_inside_fences;
        }
        obecny_blok=obecny_blok->nastepny;
    }
    return pointer_unallocated;
}

int heap_validate(void) {
    if (sterta.poczatek_sterty==NULL)
        return 2;
    if(sterta.pierwszy_blok_sterty==NULL)
        return 0;

    struct blok_sterty *obecny_blok=sterta.pierwszy_blok_sterty;
    int dlugosc_plotka=0;

    while(obecny_blok!=NULL) {
        struct blok_sterty temp;
        memcpy(&temp,obecny_blok,2*ROZMIAR_PLOTKA);
        temp=*obecny_blok;
        temp.suma_kontrolna=0;
        uint8_t *dane_temp=(uint8_t*)&temp;

        int suma_kontrolna_temp=0;
        for (int i=0;i<2*ROZMIAR_PLOTKA;i++)
            suma_kontrolna_temp=suma_kontrolna_temp+dane_temp[i];
        if(obecny_blok->suma_kontrolna!=suma_kontrolna_temp)
            return 3;
        if(obecny_blok->czy_wolny==1) {
            obecny_blok=obecny_blok->nastepny;
            continue;
        }

        char *ptr_obecny_blok=(char *)obecny_blok;
        ptr_obecny_blok=ptr_obecny_blok+2*ROZMIAR_PLOTKA;
        while (*ptr_obecny_blok==WZORZEC_PLOTKA && dlugosc_plotka<ROZMIAR_PLOTKA) {
            dlugosc_plotka++;
            ptr_obecny_blok++;
        }
        if (dlugosc_plotka!=ROZMIAR_PLOTKA)
            return 1;

        while (*(ptr_obecny_blok+obecny_blok->rozmiar)==WZORZEC_PLOTKA && dlugosc_plotka<2*ROZMIAR_PLOTKA) {
            dlugosc_plotka++;
            ptr_obecny_blok++;
        }
        if (dlugosc_plotka!=2*ROZMIAR_PLOTKA)
            return 1;

        obecny_blok=obecny_blok->nastepny;
        dlugosc_plotka=0;
    }
    return 0;
}
