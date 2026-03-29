#ifndef MYCRON_H
#define MYCRON_H

#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <pthread.h>

#define NAZWA_KOLEJKI_SERWERA "/cron_server_mq"
#define PLIK_BLOKADY_SERWERA "/tmp/cron_server.lock"

#define MAX_DLUGOSC_SCIEZKI 256
#define MAX_DLUGOSC_KOMUNIKATU_BLEDU 256
#define MAX_LICZBA_ARGUMENTOW 32
#define MAX_DLUGOSC_ARGUMENTU 128
#define MAX_LICZBA_ZADAN 20
#define MAX_DLUGOSC_NAZWY_KOLEJKI 64

typedef enum {
    KOM_DODAJ_ZADANIE_WZGLEDNE,
    KOM_DODAJ_ZADANIE_BEZWZGLEDNE,
    KOM_DODAJ_ZADANIE_CYKLICZNE,
    KOM_LISTA_ZADAN,
    KOM_ANULUJ_ZADANIE,
    KOM_ZATRZYMAJ_SERWER,
    KOM_ODPOWIEDZ_OK,
    KOM_ODPOWIEDZ_BLAD,
    KOM_ODPOWIEDZ_LISTA_ZADAN
} typ_komunikatu_t;

typedef struct {
    char sciezka_programu[MAX_DLUGOSC_SCIEZKI];
    char argumenty[MAX_LICZBA_ARGUMENTOW][MAX_DLUGOSC_ARGUMENTU];
    int liczba_argumentow;

    union {
        time_t czas_bezwzgledny;

        struct {
            int sekundy;
            int interwal_sekund;
        } wzgledny;
    } harmonogram;
} dodanie_zadania_t;

typedef struct {
    int id_zadania;
    char sciezka_programu[MAX_DLUGOSC_SCIEZKI];
    time_t zaplanowany_czas;
    int czy_cykliczne;
    int interwal_sekund;
    int czy_aktywny;
} info_zadania_t;

typedef struct {
    typ_komunikatu_t typ;
    char kolejka_odpowiedzi[MAX_DLUGOSC_NAZWY_KOLEJKI];

    union {
        dodanie_zadania_t zadanie;
        int id_zadania;
    } dane;
} komunikat_zadania_t;

typedef struct {
    typ_komunikatu_t typ;
    int status;

    union {
        int id_zadania;

        struct {
            info_zadania_t zadania[MAX_LICZBA_ZADAN];
            int liczba_zadan;
        } lista_zadan;

        char komunikat_bledu[MAX_DLUGOSC_KOMUNIKATU_BLEDU];
    } dane;
} komunikat_odpowiedzi_t;

#endif
