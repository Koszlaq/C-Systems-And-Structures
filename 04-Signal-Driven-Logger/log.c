#include "log.h"

FILE *plikLog;
char nazwaPliku[64];
pthread_mutex_t blokadaZapisu;
atomic_int koniecWatkow = 0;
volatile sig_atomic_t pending_poziom = -1;
pthread_t watekLogowania, watekZmianyPoziomu, watekDump;
atomic_int czyLogowanieAktywne = 0, aktualnyPoziomLogowania = 0, czyLoggerZainicjalizowany = 0;
sem_t semLogowanie, semZmianaPoziomu, semDump;

void handler_logowania(int sig, siginfo_t *info, void *ucontext) {
    (void) sig;(void) info;(void) ucontext;
    sem_post(&semLogowanie);
}

void handler_zmiany_poziomu(int sig, siginfo_t *info, void *ucontext) {
    (void) sig;(void) ucontext;

    pending_poziom = info->si_value.sival_int;
    sem_post(&semZmianaPoziomu);
}

void handler_dump(int sig, siginfo_t *info, void *ucontext) {
    (void) sig; (void) info; (void) ucontext;
    sem_post(&semDump);
}

void *watek_logowania(void *args) {
    (void) args;

    while (!atomic_load(&koniecWatkow)) {
        sem_wait(&semLogowanie);
        if (atomic_load(&koniecWatkow))
            break;

        if (atomic_load(&czyLogowanieAktywne) == 1)
            atomic_store(&czyLogowanieAktywne, 0);
        else
            atomic_store(&czyLogowanieAktywne, 1);

        zapisz_log(MAX, "Stan logowania: %d", atomic_load(&czyLogowanieAktywne));
    }

    return NULL;
}

void *watek_zmiany_poziomu(void *args) {
    (void) args;

    while (!atomic_load(&koniecWatkow)) {
        sem_wait(&semZmianaPoziomu);
        if (atomic_load(&koniecWatkow))
            break;

        int nowy = pending_poziom;
        pending_poziom = -1;

        if (nowy >= MIN && nowy <= MAX) {
            atomic_store(&aktualnyPoziomLogowania, nowy);
            zapisz_log(MAX, "Zmieniono poziom logowania na %d", nowy);
        }
    }

    return NULL;
}

void *watek_dump(void *args) {
    (void) args;

    while (!atomic_load(&koniecWatkow)) {
        sem_wait(&semDump);
        if (atomic_load(&koniecWatkow))
            break;

        time_t obecny_czas = time(NULL);
        struct tm czas;
        localtime_r(&obecny_czas, &czas);
        char nazwaDump[128];
        snprintf(nazwaDump, sizeof(nazwaDump),
                 "dump_%d-%02d-%02d_%02d-%02d-%02d.txt",
                 czas.tm_year + 1900,
                 czas.tm_mon + 1,
                 czas.tm_mday,
                 czas.tm_hour,
                 czas.tm_min,
                 czas.tm_sec);

        FILE *plikDump = fopen(nazwaDump, "w");
        if (plikDump) {
            fprintf(plikDump, "===== DUMP STANU APLIKACJI =====\n");
            fprintf(plikDump, "Czas: %ld\n", time(NULL));
            fprintf(plikDump, "Poziom logowania: %d\n", atomic_load(&aktualnyPoziomLogowania));
            fprintf(plikDump, "Logowanie aktywne: %d\n", atomic_load(&czyLogowanieAktywne));

            fclose(plikDump);
            zapisz_log(MAX, "Dump zapisany do pliku: %s", nazwaDump);
        }
    }

    return NULL;
}

short logger_inicjalizacja(const char *nazwa){
    if (atomic_load(&czyLoggerZainicjalizowany) == 1 || nazwa == NULL)
        return EXIT_FAILURE;

    strncpy(nazwaPliku, nazwa, sizeof(nazwaPliku) - 1);
    nazwaPliku[sizeof(nazwaPliku) - 1] = '\0';
    plikLog = fopen(nazwaPliku, "w");
    if (plikLog == NULL)
        return EXIT_FAILURE;
    fclose(plikLog);

    atomic_store(&czyLoggerZainicjalizowany, 1);
    atomic_store(&koniecWatkow, 0);
    atomic_store(&aktualnyPoziomLogowania, MIN);
    atomic_store(&czyLogowanieAktywne, 1);

    if (sem_init(&semLogowanie, 0, 0) != 0) {
        atomic_store(&czyLoggerZainicjalizowany, 0);
        return EXIT_FAILURE;
    }
    if (sem_init(&semZmianaPoziomu, 0, 0) != 0) {
        sem_destroy(&semLogowanie);
        atomic_store(&czyLoggerZainicjalizowany, 0);

        return EXIT_FAILURE;
    }
    if (sem_init(&semDump, 0, 0) != 0) {
        sem_destroy(&semZmianaPoziomu);
        sem_destroy(&semLogowanie);

        atomic_store(&czyLoggerZainicjalizowany, 0);
        return EXIT_FAILURE;
    }

    if (pthread_mutex_init(&blokadaZapisu, NULL) != 0) {
        sem_destroy(&semDump);
        sem_destroy(&semZmianaPoziomu);
        sem_destroy(&semLogowanie);

        atomic_store(&czyLoggerZainicjalizowany, 0);
        return EXIT_FAILURE;
    }
    if (pthread_create(&watekLogowania, NULL, watek_logowania, NULL) != 0) {
        pthread_mutex_destroy(&blokadaZapisu);
        sem_destroy(&semDump);
        sem_destroy(&semZmianaPoziomu);
        sem_destroy(&semLogowanie);

        atomic_store(&czyLoggerZainicjalizowany, 0);
        return EXIT_FAILURE;
    }
    if (pthread_create(&watekZmianyPoziomu, NULL, watek_zmiany_poziomu, NULL) != 0) {
        atomic_store(&koniecWatkow, 1);
        sem_post(&semLogowanie);
        pthread_join(watekLogowania, NULL);

        pthread_mutex_destroy(&blokadaZapisu);
        sem_destroy(&semDump);
        sem_destroy(&semZmianaPoziomu);
        sem_destroy(&semLogowanie);
        atomic_store(&czyLoggerZainicjalizowany, 0);

        return EXIT_FAILURE;
    }
    if (pthread_create(&watekDump, NULL, watek_dump, NULL) != 0) {
        atomic_store(&koniecWatkow, 1);

        sem_post(&semLogowanie);
        sem_post(&semZmianaPoziomu);

        pthread_join(watekLogowania, NULL);
        pthread_join(watekZmianyPoziomu, NULL);

        pthread_mutex_destroy(&blokadaZapisu);
        sem_destroy(&semDump);
        sem_destroy(&semZmianaPoziomu);
        sem_destroy(&semLogowanie);

        atomic_store(&czyLoggerZainicjalizowany, 0);
        return EXIT_FAILURE;
    }

    struct sigaction akt;
    sigfillset(&akt.sa_mask);
    akt.sa_flags = SA_SIGINFO;
    akt.sa_sigaction = handler_zmiany_poziomu;
    sigaction(SYGNAL_ZMIANY_POZIOMU, &akt, NULL);
    akt.sa_sigaction = handler_logowania;
    sigaction(SYGNAL_LOGOWANIE, &akt, NULL);
    akt.sa_sigaction = handler_dump;
    sigaction(SYGNAL_DUMP, &akt, NULL);

    return EXIT_SUCCESS;
}

void logger_czyszczenie(void) {
    atomic_store(&koniecWatkow, 1);

    sem_post(&semLogowanie);
    sem_post(&semZmianaPoziomu);
    sem_post(&semDump);

    pthread_join(watekLogowania, NULL);
    pthread_join(watekZmianyPoziomu, NULL);
    pthread_join(watekDump, NULL);

    sem_destroy(&semLogowanie);
    sem_destroy(&semZmianaPoziomu);
    sem_destroy(&semDump);

    pthread_mutex_destroy(&blokadaZapisu);

    atomic_store(&czyLoggerZainicjalizowany, 0);
}

void zapisz_log(const int poziom, const char *format, ...) {
    if (atomic_load(&czyLoggerZainicjalizowany) == 0)
        return;

    pthread_mutex_lock(&blokadaZapisu);
    if (atomic_load(&czyLogowanieAktywne) == 1 && atomic_load(&aktualnyPoziomLogowania) >= poziom) {
        plikLog = fopen(nazwaPliku, "a");
        if (plikLog!=NULL) {
            switch (poziom) {
                case MIN: fprintf(plikLog, "MIN: ");
                    break;
                case STANDARD: fprintf(plikLog, "STD: ");
                    break;
                case MAX: fprintf(plikLog, "MAX: ");
                    break;
                default: fprintf(plikLog, "NIEZNANY(%d): ", poziom);
                    break;
            }

            va_list args;
            va_start(args, format);
            vfprintf(plikLog, format, args);
            va_end(args);

            fprintf(plikLog, "\n");
            fclose(plikLog);
        }
    }
    pthread_mutex_unlock(&blokadaZapisu);
}
