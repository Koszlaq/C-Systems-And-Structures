#define _GNU_SOURCE

#include "MyCron.h"

#include <sys/stat.h>
#include <sys/file.h>
#include <spawn.h>
#include <pthread.h>

typedef struct {
    int id_zadania;
    int czy_aktywny;
    char sciezka_programu[MAX_DLUGOSC_SCIEZKI];
    char *argumenty[MAX_LICZBA_ARGUMENTOW + 1];
    time_t harmonogram;
    int czy_cykliczne;
    int interwal_sekund;
    timer_t timer;
    int liczba_argumentow;
} zadania_wewnetrzne_t;

extern char **environ;
static zadania_wewnetrzne_t zadania[MAX_LICZBA_ZADAN];
static pthread_mutex_t mutex_zadan = PTHREAD_MUTEX_INITIALIZER;
static mqd_t kolejka_serwera = -1;
static int nastepne_id_zadania = 1, czy_serwer_aktywny = 1, deskryptor_blokady = -1;

void wykonaj_zadanie(zadania_wewnetrzne_t *zadanie) {
    pid_t pid_dziecka;

    zapisz_log(STANDARD, "Uruchamianie zadania ID=%d: %s", zadanie->id_zadania, zadanie->sciezka_programu);

    int status = posix_spawn(&pid_dziecka, zadanie->sciezka_programu, NULL, NULL, zadanie->argumenty, environ);
    if (status == 0)
        zapisz_log(STANDARD, "Zadanie ID=%d uruchomione, PID=%d", zadanie->id_zadania, pid_dziecka);
    else
        zapisz_log(MIN, "Błąd uruchamiania zadania ID=%d: %s", zadanie->id_zadania, strerror(status));
}

void handler_timera(union sigval sv) {
    zadania_wewnetrzne_t *zadanie = (zadania_wewnetrzne_t *) sv.sival_ptr;

    pthread_mutex_lock(&mutex_zadan);
    if (zadanie == NULL || zadanie->czy_aktywny == 0) {
        pthread_mutex_unlock(&mutex_zadan);
        return;
    }
    int id_zadania = zadanie->id_zadania, czy_cykliczne = zadanie->czy_cykliczne;
    pthread_mutex_unlock(&mutex_zadan);

    wykonaj_zadanie(zadanie);

    pthread_mutex_lock(&mutex_zadan);
    if (zadanie->czy_aktywny == 0) {
        pthread_mutex_unlock(&mutex_zadan);
        return;
    }
    if (czy_cykliczne == 0) {
        zadanie->czy_aktywny = 0;
        for (int i = 0; i < zadanie->liczba_argumentow; i++) {
            free(zadanie->argumenty[i]);
            zadanie->argumenty[i] = NULL;
        }
        zadanie->liczba_argumentow = 0;
        timer_delete(zadanie->timer);
        zapisz_log(MAX, "Zadanie jednorazowe ID=%d zakończone i wyczyszczone", id_zadania);
    } else {
        zadanie->harmonogram += zadanie->interwal_sekund;
        zapisz_log(MAX, "Zadanie okresowe ID=%d, następne uruchomienie za %ds", zadanie->id_zadania, zadanie->interwal_sekund);
    }
    pthread_mutex_unlock(&mutex_zadan);
}

int dodanie_zadania(dodanie_zadania_t *dane_od_klienta, typ_komunikatu_t typ_komunikatu, int *nowe_zadanie_id) {
    pthread_mutex_lock(&mutex_zadan);

    int wolne_miejsce = -1;
    for (int i = 0; i < MAX_LICZBA_ZADAN; i++) {
        if (zadania[i].czy_aktywny == 0) {
            wolne_miejsce = i;
            break;
        }
    }
    if (wolne_miejsce == -1) {
        pthread_mutex_unlock(&mutex_zadan);
        zapisz_log(MIN, "Brak wolnych slotów dla zadań");
        return -1;
    }

    zadania_wewnetrzne_t *zadanie = &zadania[wolne_miejsce];
    zadanie->id_zadania = nastepne_id_zadania++;
    zadanie->czy_aktywny = 1;
    zadanie->liczba_argumentow = dane_od_klienta->liczba_argumentow;
    strncpy(zadanie->sciezka_programu, dane_od_klienta->sciezka_programu, MAX_DLUGOSC_SCIEZKI - 1);
    for (int i = 0; i < dane_od_klienta->liczba_argumentow && i < MAX_LICZBA_ARGUMENTOW; i++) {
        zadanie->argumenty[i] = malloc(MAX_DLUGOSC_ARGUMENTU);
        if (zadanie->argumenty[i] == NULL) {
            for (int j = 0; j < i; j++)
                free(zadanie->argumenty[j]);
            zadanie->czy_aktywny = 0;
            pthread_mutex_unlock(&mutex_zadan);
            return -1;
        }
        strncpy(zadanie->argumenty[i], dane_od_klienta->argumenty[i], MAX_DLUGOSC_ARGUMENTU - 1);
    }
    zadanie->argumenty[dane_od_klienta->liczba_argumentow] = NULL;

    time_t obecny_czas = time(NULL);
    int opoznienie;
    switch (typ_komunikatu) {
        case KOM_DODAJ_ZADANIE_BEZWZGLEDNE:
            zadanie->harmonogram = dane_od_klienta->harmonogram.czas_bezwzgledny;
            zadanie->czy_cykliczne = 0;
            zadanie->interwal_sekund = 0;
            opoznienie = (int) difftime(zadanie->harmonogram, obecny_czas);
            if (opoznienie < 0)
                opoznienie = 0;
            break;

        case KOM_DODAJ_ZADANIE_WZGLEDNE:
            opoznienie = dane_od_klienta->harmonogram.wzgledny.sekundy;
            zadanie->harmonogram = obecny_czas + opoznienie;
            zadanie->czy_cykliczne = 0;
            zadanie->interwal_sekund = 0;
            break;

        case KOM_DODAJ_ZADANIE_CYKLICZNE:
            opoznienie = dane_od_klienta->harmonogram.wzgledny.sekundy;
            zadanie->harmonogram = obecny_czas + opoznienie;
            zadanie->czy_cykliczne = 1;
            zadanie->interwal_sekund = dane_od_klienta->harmonogram.wzgledny.interwal_sekund;
            break;

        default:
            for (int i = 0; i < zadanie->liczba_argumentow; i++)
                free(zadanie->argumenty[i]);
            zadanie->czy_aktywny = 0;
            pthread_mutex_unlock(&mutex_zadan);
            return -1;
    }

    struct sigevent sev;
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = handler_timera;
    sev.sigev_value.sival_ptr = zadanie;
    sev.sigev_notify_attributes = NULL;

    if (timer_create(CLOCK_REALTIME, &sev, &zadanie->timer) == -1) {
        zapisz_log(MIN, "Błąd tworzenia timera: %s", strerror(errno));
        zadanie->czy_aktywny = 0;
        for (int i = 0; i < zadanie->liczba_argumentow; i++)
            free(zadanie->argumenty[i]);
        pthread_mutex_unlock(&mutex_zadan);
        return -1;
    }

    struct itimerspec its;
    its.it_value.tv_sec = opoznienie;
    its.it_value.tv_nsec = 0;
    if (zadanie->czy_cykliczne) {
        its.it_interval.tv_sec = zadanie->interwal_sekund;
        its.it_interval.tv_nsec = 0;
    } else {
        its.it_interval.tv_sec = 0;
        its.it_interval.tv_nsec = 0;
    }

    if (timer_settime(zadanie->timer, 0, &its, NULL) == -1) {
        zapisz_log(MIN, "Błąd ustawiania timera: %s", strerror(errno));
        timer_delete(zadanie->timer);
        zadanie->czy_aktywny = 0;
        for (int i = 0; i < zadanie->liczba_argumentow; i++)
            free(zadanie->argumenty[i]);
        pthread_mutex_unlock(&mutex_zadan);
        return -1;
    }

    *nowe_zadanie_id = zadanie->id_zadania;
    zapisz_log(STANDARD, "Dodano zadanie ID=%d, program=%s, opóźnienie=%ds, czy_cykliczne=%d", zadanie->id_zadania, zadanie->sciezka_programu, opoznienie, zadanie->czy_cykliczne);

    pthread_mutex_unlock(&mutex_zadan);
    return 0;
}

int anulowanie_zadania(int id_zadania) {
    pthread_mutex_lock(&mutex_zadan);

    for (int i = 0; i < MAX_LICZBA_ZADAN; i++) {
        if (zadania[i].czy_aktywny == 1 && zadania[i].id_zadania == id_zadania) {
            timer_delete(zadania[i].timer);
            zadania[i].czy_aktywny = 0;

            for (int j = 0; j < zadania[i].liczba_argumentow; j++) {
                free(zadania[i].argumenty[j]);
                zadania[i].argumenty[j] = NULL;
            }
            zadania[i].liczba_argumentow = 0;

            zapisz_log(STANDARD, "Anulowano zadanie ID=%d", id_zadania);
            pthread_mutex_unlock(&mutex_zadan);
            return 0;
        }
    }

    pthread_mutex_unlock(&mutex_zadan);
    zapisz_log(MIN, "Nie znaleziono zadania ID=%d do anulowania", id_zadania);
    return -1;
}

int lista_zadan(info_zadania_t *wyjscie_zadan, int *liczba) {
    pthread_mutex_lock(&mutex_zadan);

    *liczba = 0;
    for (int i = 0; i < MAX_LICZBA_ZADAN; i++) {
        if (zadania[i].czy_aktywny) {
            wyjscie_zadan[*liczba].id_zadania = zadania[i].id_zadania;
            strncpy(wyjscie_zadan[*liczba].sciezka_programu, zadania[i].sciezka_programu, MAX_DLUGOSC_SCIEZKI - 1);
            wyjscie_zadan[*liczba].zaplanowany_czas = zadania[i].harmonogram;
            wyjscie_zadan[*liczba].czy_cykliczne = zadania[i].czy_cykliczne;
            wyjscie_zadan[*liczba].interwal_sekund = zadania[i].interwal_sekund;
            wyjscie_zadan[*liczba].czy_aktywny = zadania[i].czy_aktywny;
            (*liczba)++;
        }
    }

    pthread_mutex_unlock(&mutex_zadan);
    return 0;
}

void czyszczenie_wszystkich_zadan(void) {
    pthread_mutex_lock(&mutex_zadan);

    for (int i = 0; i < MAX_LICZBA_ZADAN; i++) {
        if (zadania[i].czy_aktywny) {
            timer_delete(zadania[i].timer);
            zadania[i].czy_aktywny = 0;

            for (int j = 0; j < zadania[i].liczba_argumentow; j++) {
                free(zadania[i].argumenty[j]);
                zadania[i].argumenty[j] = NULL;
            }
            zadania[i].liczba_argumentow = 0;
        }
    }

    pthread_mutex_unlock(&mutex_zadan);
    zapisz_log(STANDARD, "Wszystkie zadania anulowane");
}

void petla_serwera(void) {
    komunikat_zadania_t zadanie;
    komunikat_odpowiedzi_t odpowiedz;

    while (czy_serwer_aktywny) {
        if (mq_receive(kolejka_serwera, (char *) &zadanie, sizeof(zadanie), NULL) == -1) {
            if (errno == EINTR) continue;
            zapisz_log(MIN, "Błąd mq_receive: %s", strerror(errno));
            break;
        }

        memset(&odpowiedz, 0, sizeof(odpowiedz));

        mqd_t kolejka_odpowiedzi = mq_open(zadanie.kolejka_odpowiedzi, O_WRONLY);
        if (kolejka_odpowiedzi == -1) {
            zapisz_log(MIN, "Nie można otworzyć kolejki odpowiedzi: %s", strerror(errno));
            continue;
        }

        switch (zadanie.typ) {
            case KOM_DODAJ_ZADANIE_BEZWZGLEDNE:
            case KOM_DODAJ_ZADANIE_WZGLEDNE:
            case KOM_DODAJ_ZADANIE_CYKLICZNE: {
                int id_zadania;
                if (dodanie_zadania(&zadanie.dane.zadanie, zadanie.typ, &id_zadania) == 0) {
                    odpowiedz.typ = KOM_ODPOWIEDZ_OK;
                    odpowiedz.status = 0;
                    odpowiedz.dane.id_zadania = id_zadania;
                } else {
                    odpowiedz.typ = KOM_ODPOWIEDZ_BLAD;
                    odpowiedz.status = -1;
                    strncpy(odpowiedz.dane.komunikat_bledu, "Nie można dodać zadania", sizeof(odpowiedz.dane.komunikat_bledu) - 1);
                }
                break;
            }

            case KOM_LISTA_ZADAN:
                odpowiedz.typ = KOM_ODPOWIEDZ_LISTA_ZADAN;
                lista_zadan(odpowiedz.dane.lista_zadan.zadania, &odpowiedz.dane.lista_zadan.liczba_zadan);
                odpowiedz.status = 0;
                break;

            case KOM_ANULUJ_ZADANIE:
                if (anulowanie_zadania(zadanie.dane.id_zadania) == 0) {
                    odpowiedz.typ = KOM_ODPOWIEDZ_OK;
                    odpowiedz.status = 0;
                } else {
                    odpowiedz.typ = KOM_ODPOWIEDZ_BLAD;
                    odpowiedz.status = -1;
                    strncpy(odpowiedz.dane.komunikat_bledu, "Nie znaleziono zadania", sizeof(odpowiedz.dane.komunikat_bledu) - 1);
                }
                break;

            case KOM_ZATRZYMAJ_SERWER:
                odpowiedz.typ = KOM_ODPOWIEDZ_OK;
                odpowiedz.status = 0;
                mq_send(kolejka_odpowiedzi, (const char *) &odpowiedz, sizeof(odpowiedz), 0);
                mq_close(kolejka_odpowiedzi);
                czy_serwer_aktywny = 0;
                continue;

            default:
                odpowiedz.typ = KOM_ODPOWIEDZ_BLAD;
                odpowiedz.status = -1;
                strncpy(odpowiedz.dane.komunikat_bledu, "Nieznany typ komunikatu",
                        sizeof(odpowiedz.dane.komunikat_bledu) - 1);
                break;
        }

        mq_send(kolejka_odpowiedzi, (const char *) &odpowiedz, sizeof(odpowiedz), 0);
        mq_close(kolejka_odpowiedzi);
    }
}

int czy_serwer_dziala(void) {
    deskryptor_blokady = open(PLIK_BLOKADY_SERWERA, O_CREAT | O_RDWR, 0666);
    if (deskryptor_blokady == -1) {
        return -1;
    }

    if (flock(deskryptor_blokady, LOCK_EX | LOCK_NB) == -1) {
        close(deskryptor_blokady);
        return -1;
    }

    return 0;
}

int uruchom_serwer(void) {
    if (logger_inicjalizacja("/tmp/cron_server.log") != EXIT_SUCCESS) {
        fprintf(stderr, "Nie można zainicjalizować loggera\n");
        flock(deskryptor_blokady, LOCK_UN);
        close(deskryptor_blokady);
        return EXIT_FAILURE;
    }

    mq_unlink(NAZWA_KOLEJKI_SERWERA);

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(komunikat_zadania_t);
    attr.mq_curmsgs = 0;
    kolejka_serwera = mq_open(NAZWA_KOLEJKI_SERWERA, O_CREAT | O_RDONLY, 0666, &attr);
    if (kolejka_serwera == -1) {
        zapisz_log(MIN, "Nie można utworzyć kolejki: %s", strerror(errno));
        logger_czyszczenie();
        flock(deskryptor_blokady, LOCK_UN);
        close(deskryptor_blokady);
        return EXIT_FAILURE;
    }

    zapisz_log(STANDARD, "Serwer MyCron uruchomiony");
    printf("Serwer MyCron uruchomiony.\n");
    printf("Logi zapisywane w /tmp/cron_server.log\n");

    petla_serwera();

    czyszczenie_wszystkich_zadan();
    mq_close(kolejka_serwera);
    mq_unlink(NAZWA_KOLEJKI_SERWERA);

    zapisz_log(STANDARD, "Serwer MyCron zatrzymany");
    logger_czyszczenie();

    flock(deskryptor_blokady, LOCK_UN);
    close(deskryptor_blokady);
    unlink(PLIK_BLOKADY_SERWERA);

    return EXIT_SUCCESS;
}
