#include "cron_client.h"

void wypisz_komendy(const char *prog_name) {
    printf("Użycie:\n");
    printf("  %s                                                      - uruchom serwer\n", prog_name);
    printf("  %s lista                                                - wyświetl listę zadań\n", prog_name);
    printf("  %s pomoc                                                - wyświetl listę komend\n", prog_name);
    printf("  %s dodaj-wzglednie <sekundy> <program> [args...]        - dodaj zadanie względne\n", prog_name);
    printf("  %s dodaj-bezwzglednie <czas> <program> [args...]        - dodaj zadanie bezwzględne\n", prog_name);
    printf("  %s dodaj-cyklicznie <start> <interwal> <prog> [args...] - dodaj zadanie okresowe\n", prog_name);
    printf("  %s anuluj <id_zadania>                                  - anuluj zadanie\n", prog_name);
    printf("  %s wylacz                                               - zatrzymaj serwer\n", prog_name);
}

int klient_dodanie_zadania(typ_komunikatu_t typ, int argc, char *argv[]) {
    komunikat_zadania_t zadanie;
    komunikat_odpowiedzi_t odpowiedz;

    memset(&zadanie, 0, sizeof(zadanie));
    zadanie.typ = typ;
    snprintf(zadanie.kolejka_odpowiedzi, MAX_DLUGOSC_NAZWY_KOLEJKI, "/cron_reply_%d", getpid());

    int poczatek_argumentow;
    switch (typ) {
        case KOM_DODAJ_ZADANIE_WZGLEDNE:
            if (argc < 3) {
                fprintf(stderr, "Za mało argumentów dla dodaj-wzglednie\n");
                return -1;
            }

            char *endptr1;
            long sekundy = strtol(argv[1], &endptr1, 10);
            if (*endptr1 != '\0' || endptr1 == argv[1]) {
                fprintf(stderr, "Liczba sekund musi być liczbą całkowitą\n");
                fprintf(stderr, "Użyj: dodaj-wzglednie <sekundy> <program> [args...]\n");
                return -1;
            }
            if (sekundy < 0) {
                fprintf(stderr, "Liczba sekund nie może być ujemna\n");
                fprintf(stderr, "Użyj: dodaj-wzglednie <sekundy> <program> [args...]\n");
                return -1;
            }

            zadanie.dane.zadanie.harmonogram.wzgledny.sekundy = atoi(argv[1]);
            zadanie.dane.zadanie.harmonogram.wzgledny.interwal_sekund = 0;
            poczatek_argumentow = 2;
            break;

        case KOM_DODAJ_ZADANIE_BEZWZGLEDNE:
            if (argc < 3) {
                fprintf(stderr, "Za mało argumentów dla dodaj-bezwzglednie\n");
                return -1;
            }

            char *endptr2;
            long timestamp = strtol(argv[1], &endptr2, 10);
            if (*endptr2 != '\0' || endptr2 == argv[1]) {
                fprintf(stderr, "Timestamp musi być liczbą całkowitą\n");
                fprintf(stderr, "Użyj: dodaj-bezwzglednie <timestamp> <program> [args...]\n");
                return -1;
            }
            if (timestamp < 0) {
                fprintf(stderr, "Timestamp nie może być ujemny\n");
                fprintf(stderr, "Użyj: dodaj-bezwzglednie <timestamp> <program> [args...]\n");
                return -1;
            }

            zadanie.dane.zadanie.harmonogram.czas_bezwzgledny = (time_t) atol(argv[1]);
            poczatek_argumentow = 2;
            break;

        case KOM_DODAJ_ZADANIE_CYKLICZNE:
            if (argc < 4) {
                fprintf(stderr, "Za mało argumentów dla dodaj-cyklicznie\n");
                return -1;
            }

            char *endptr3;
            long start_sek = strtol(argv[1], &endptr3, 10);
            if (*endptr3 != '\0' || endptr3 == argv[1]) {
                fprintf(stderr, "Start (sekundy) musi być liczbą całkowitą\n");
                fprintf(stderr, "Użyj: dodaj-cyklicznie <start> <interwał> <program> [args...]\n");
                return -1;
            }
            if (start_sek < 0) {
                fprintf(stderr, "Start (sekundy) nie może być ujemny\n");
                fprintf(stderr, "Użyj: dodaj-cyklicznie <start> <interwał> <program> [args...]\n");
                return -1;
            }

            char *endptr4;
            long interwal = strtol(argv[2], &endptr4, 10);
            if (*endptr4 != '\0' || endptr4 == argv[2]) {
                fprintf(stderr, "Interwał musi być liczbą całkowitą\n");
                fprintf(stderr, "Użyj: dodaj-cyklicznie <start> <interwał> <program> [args...]\n");
                return -1;
            }
            if (interwal <= 0) {
                fprintf(stderr, "Interwał musi być liczbą dodatnią (większą od 0)\n");
                fprintf(stderr, "Użyj: dodaj-cyklicznie <start> <interwał> <program> [args...]\n");
                return -1;
            }

            zadanie.dane.zadanie.harmonogram.wzgledny.sekundy = atoi(argv[1]);
            zadanie.dane.zadanie.harmonogram.wzgledny.interwal_sekund = atoi(argv[2]);
            poczatek_argumentow = 3;
            break;

        default:
            return -1;
    }

    strncpy(zadanie.dane.zadanie.sciezka_programu, argv[poczatek_argumentow], MAX_DLUGOSC_SCIEZKI - 1);
    zadanie.dane.zadanie.liczba_argumentow = 0;
    for (int i = poczatek_argumentow; i < argc && zadanie.dane.zadanie.liczba_argumentow < MAX_LICZBA_ARGUMENTOW; i++) {
        strncpy(zadanie.dane.zadanie.argumenty[zadanie.dane.zadanie.liczba_argumentow],
                argv[i], MAX_DLUGOSC_ARGUMENTU - 1);
        zadanie.dane.zadanie.liczba_argumentow++;
    }

    mqd_t serwer_mq = mq_open(NAZWA_KOLEJKI_SERWERA, O_WRONLY);
    if (serwer_mq == -1) {
        fprintf(stderr, "Nie można połączyć się z serwerem.\n");
        fprintf(stderr, "Uruchom serwer przez uruchomienie programu bez argumentów.\n");
        return -1;
    }
    mq_unlink(zadanie.kolejka_odpowiedzi);
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(komunikat_odpowiedzi_t);
    attr.mq_curmsgs = 0;
    mqd_t odpowiedz_mq = mq_open(zadanie.kolejka_odpowiedzi, O_CREAT | O_RDONLY, 0666, &attr);
    if (odpowiedz_mq == -1) {
        fprintf(stderr, "Nie można utworzyć kolejki odpowiedzi: %s\n", strerror(errno));
        mq_close(serwer_mq);
        return -1;
    }
    if (mq_send(serwer_mq, (const char *) &zadanie, sizeof(zadanie), 0) == -1) {
        fprintf(stderr, "Błąd wysyłania komunikatu: %s\n", strerror(errno));
        mq_close(serwer_mq);
        mq_close(odpowiedz_mq);
        mq_unlink(zadanie.kolejka_odpowiedzi);
        return -1;
    }
    if (mq_receive(odpowiedz_mq, (char *) &odpowiedz, sizeof(odpowiedz), NULL) == -1) {
        fprintf(stderr, "Błąd odbierania odpowiedzi: %s\n", strerror(errno));
        mq_close(serwer_mq);
        mq_close(odpowiedz_mq);
        mq_unlink(zadanie.kolejka_odpowiedzi);
        return -1;
    }
    mq_close(serwer_mq);
    mq_close(odpowiedz_mq);
    mq_unlink(zadanie.kolejka_odpowiedzi);

    if (odpowiedz.status == 0) {
        printf("Zadanie dodane pomyślnie, ID=%d\n", odpowiedz.dane.id_zadania);
        return 0;
    } else {
        fprintf(stderr, "Błąd: %s\n", odpowiedz.dane.komunikat_bledu);
        return -1;
    }
}

int klient_lista_zadan(void) {
    komunikat_zadania_t zadanie;
    komunikat_odpowiedzi_t odpowiedz;

    memset(&zadanie, 0, sizeof(zadanie));
    zadanie.typ = KOM_LISTA_ZADAN;
    snprintf(zadanie.kolejka_odpowiedzi, MAX_DLUGOSC_NAZWY_KOLEJKI, "/cron_reply_%d", getpid());

    mqd_t serwer_mq = mq_open(NAZWA_KOLEJKI_SERWERA, O_WRONLY);
    if (serwer_mq == -1) {
        fprintf(stderr, "Nie można połączyć się z serwerem.\n");
        fprintf(stderr, "Uruchom serwer przez uruchomienie programu bez argumentów.\n");
        return -1;
    }
    mq_unlink(zadanie.kolejka_odpowiedzi);
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(komunikat_odpowiedzi_t);
    attr.mq_curmsgs = 0;
    mqd_t odpowiedz_mq = mq_open(zadanie.kolejka_odpowiedzi, O_CREAT | O_RDONLY, 0666, &attr);
    if (odpowiedz_mq == -1) {
        fprintf(stderr, "Nie można utworzyć kolejki odpowiedzi: %s\n", strerror(errno));
        mq_close(serwer_mq);
        return -1;
    }
    if (mq_send(serwer_mq, (const char *) &zadanie, sizeof(zadanie), 0) == -1) {
        fprintf(stderr, "Błąd wysyłania komunikatu: %s\n", strerror(errno));
        mq_close(serwer_mq);
        mq_close(odpowiedz_mq);
        mq_unlink(zadanie.kolejka_odpowiedzi);
        return -1;
    }
    if (mq_receive(odpowiedz_mq, (char *) &odpowiedz, sizeof(odpowiedz), NULL) == -1) {
        fprintf(stderr, "Błąd odbierania odpowiedzi: %s\n", strerror(errno));
        mq_close(serwer_mq);
        mq_close(odpowiedz_mq);
        mq_unlink(zadanie.kolejka_odpowiedzi);
        return -1;
    }
    mq_close(serwer_mq);
    mq_close(odpowiedz_mq);
    mq_unlink(zadanie.kolejka_odpowiedzi);

    printf("=== Lista zaplanowanych zadań ===\n");
    printf("Liczba aktywnych zadań: %d\n", odpowiedz.dane.lista_zadan.liczba_zadan);
    if (odpowiedz.dane.lista_zadan.liczba_zadan == 0)
        printf("Brak aktywnych zadań.\n");
    else {
        time_t obecny_czas = time(NULL);

        for (int i = 0; i < odpowiedz.dane.lista_zadan.liczba_zadan; i++) {
            info_zadania_t *zadanie_info = &odpowiedz.dane.lista_zadan.zadania[i];

            printf("ID: %d\n", zadanie_info->id_zadania);
            printf("  Program: %s\n", zadanie_info->sciezka_programu);

            char czas[64];
            struct tm *czas_info = localtime(&zadanie_info->zaplanowany_czas);
            strftime(czas, sizeof(czas), "%Y-%m-%d %H:%M:%S", czas_info);
            printf("  Zaplanowany czas: %s", czas);

            int sekundy_do_konca = (int) difftime(zadanie_info->zaplanowany_czas, obecny_czas);
            if (sekundy_do_konca > 0)
                printf(" (za %d sekund)\n", sekundy_do_konca);
            else
                printf(" (w trakcie wykonywania)\n");

            if (zadanie_info->czy_cykliczne)
                printf("  Typ: Okresowe (co %d sekund)\n", zadanie_info->interwal_sekund);
            else
                printf("  Typ: Jednorazowe\n");

            printf("  Status: %s\n", zadanie_info->czy_aktywny ? "Aktywne" : "Nieaktywne");
        }
    }
    return 0;
}

int klient_anulowanie_zadania(const char *id_str) {
    char *ptr;
    long zadanie_id = strtol(id_str, &ptr, 10);

    if (*ptr != '\0' || ptr == id_str) {
        fprintf(stderr, "ID zadania musi być liczbą całkowitą\n");
        fprintf(stderr, "Użyj: anuluj <id_zadania>\n");
        return -1;
    }
    if (zadanie_id <= 0) {
        fprintf(stderr, "ID zadania musi być liczbą dodatnią\n");
        fprintf(stderr, "Użyj: anuluj <id_zadania>\n");
        return -1;
    }

    int id_zadania = (int)zadanie_id;
    komunikat_zadania_t zadanie;
    komunikat_odpowiedzi_t odpowiedz;

    memset(&zadanie, 0, sizeof(zadanie));
    zadanie.typ = KOM_ANULUJ_ZADANIE;
    zadanie.dane.id_zadania = id_zadania;
    snprintf(zadanie.kolejka_odpowiedzi, MAX_DLUGOSC_NAZWY_KOLEJKI, "/cron_reply_%d", getpid());

    mqd_t serwer_mq = mq_open(NAZWA_KOLEJKI_SERWERA, O_WRONLY);
    if (serwer_mq == -1) {
        fprintf(stderr, "Nie można połączyć się z serwerem.\n");
        fprintf(stderr, "Uruchom serwer przez uruchomienie programu bez argumentów.\n");
        return -1;
    }
    mq_unlink(zadanie.kolejka_odpowiedzi);
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(komunikat_odpowiedzi_t);
    attr.mq_curmsgs = 0;
    mqd_t odpowiedz_mq = mq_open(zadanie.kolejka_odpowiedzi, O_CREAT | O_RDONLY, 0666, &attr);
    if (odpowiedz_mq == -1) {
        fprintf(stderr, "Nie można utworzyć kolejki odpowiedzi: %s\n", strerror(errno));
        mq_close(serwer_mq);
        return -1;
    }
    if (mq_send(serwer_mq, (const char *) &zadanie, sizeof(zadanie), 0) == -1) {
        fprintf(stderr, "Błąd wysyłania komunikatu: %s\n", strerror(errno));
        mq_close(serwer_mq);
        mq_close(odpowiedz_mq);
        mq_unlink(zadanie.kolejka_odpowiedzi);
        return -1;
    }
    if (mq_receive(odpowiedz_mq, (char *) &odpowiedz, sizeof(odpowiedz), NULL) == -1) {
        fprintf(stderr, "Błąd odbierania odpowiedzi: %s\n", strerror(errno));
        mq_close(serwer_mq);
        mq_close(odpowiedz_mq);
        mq_unlink(zadanie.kolejka_odpowiedzi);
        return -1;
    }
    mq_close(serwer_mq);
    mq_close(odpowiedz_mq);
    mq_unlink(zadanie.kolejka_odpowiedzi);

    if (odpowiedz.status == 0) {
        printf("Zadanie ID=%d anulowane pomyślnie\n", id_zadania);
        return 0;
    } else {
        fprintf(stderr, "Błąd: %s\n", odpowiedz.dane.komunikat_bledu);
        return -1;
    }
}

int klient_zatrzymanie_serwera(void) {
    komunikat_zadania_t zadanie;
    komunikat_odpowiedzi_t odpowiedz;

    memset(&zadanie, 0, sizeof(zadanie));
    zadanie.typ = KOM_ZATRZYMAJ_SERWER;
    snprintf(zadanie.kolejka_odpowiedzi, MAX_DLUGOSC_NAZWY_KOLEJKI, "/cron_reply_%d", getpid());

    mqd_t serwer_mq = mq_open(NAZWA_KOLEJKI_SERWERA, O_WRONLY);
    if (serwer_mq == -1) {
        fprintf(stderr, "Nie można połączyć się z serwerem.\n");
        fprintf(stderr, "Uruchom serwer przez uruchomienie programu bez argumentów.\n");
        return -1;
    }
    mq_unlink(zadanie.kolejka_odpowiedzi);
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(komunikat_odpowiedzi_t);
    attr.mq_curmsgs = 0;
    mqd_t odpowiedz_mq = mq_open(zadanie.kolejka_odpowiedzi, O_CREAT | O_RDONLY, 0666, &attr);
    if (odpowiedz_mq == -1) {
        fprintf(stderr, "Nie można utworzyć kolejki odpowiedzi: %s\n", strerror(errno));
        mq_close(serwer_mq);
        return -1;
    }
    if (mq_send(serwer_mq, (const char *) &zadanie, sizeof(zadanie), 0) == -1) {
        fprintf(stderr, "Błąd wysyłania komunikatu: %s\n", strerror(errno));
        mq_close(serwer_mq);
        mq_close(odpowiedz_mq);
        mq_unlink(zadanie.kolejka_odpowiedzi);
        return -1;
    }
    if (mq_receive(odpowiedz_mq, (char *) &odpowiedz, sizeof(odpowiedz), NULL) == -1) {
        fprintf(stderr, "Błąd odbierania odpowiedzi: %s\n", strerror(errno));
        mq_close(serwer_mq);
        mq_close(odpowiedz_mq);
        mq_unlink(zadanie.kolejka_odpowiedzi);
        return -1;
    }
    mq_close(serwer_mq);
    mq_close(odpowiedz_mq);
    mq_unlink(zadanie.kolejka_odpowiedzi);

    printf("Serwer zatrzymany\n");
    return 0;
}
