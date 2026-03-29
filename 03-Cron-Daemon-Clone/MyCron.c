#include "MyCron.h"
#include "cron_client.h"
#include "cron_server.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        if (czy_serwer_dziala() != 0) {
            fprintf(stderr, "Serwer już działa.\n");
            fprintf(stderr, "Użyj '%s lista' aby zobaczyć zadania.\n", argv[0]);
            return EXIT_FAILURE;
        }
        return uruchom_serwer();
    }

    mqd_t czy_serwer_dziala_mq = mq_open(NAZWA_KOLEJKI_SERWERA, O_WRONLY | O_NONBLOCK);
    if (czy_serwer_dziala_mq == -1) {
        fprintf(stderr, "Nie można połączyć się z serwerm.\n");
        fprintf(stderr, "Uruchom serwer przez uruchomienie programu bez argumentów.\n");
        return EXIT_FAILURE;
    }
    mq_close(czy_serwer_dziala_mq);

    int wynik = EXIT_SUCCESS;
    if (strcmp(argv[1], "dodaj-wzglednie") == 0) {
        if (klient_dodanie_zadania(KOM_DODAJ_ZADANIE_WZGLEDNE, argc - 1, argv + 1) != 0)
            wynik = EXIT_FAILURE;
    } else if (strcmp(argv[1], "dodaj-bezwzglednie") == 0) {
        if (klient_dodanie_zadania(KOM_DODAJ_ZADANIE_BEZWZGLEDNE, argc - 1, argv + 1) != 0)
            wynik = EXIT_FAILURE;
    } else if (strcmp(argv[1], "dodaj-cyklicznie") == 0) {
        if (klient_dodanie_zadania(KOM_DODAJ_ZADANIE_CYKLICZNE, argc - 1, argv + 1) != 0)
            wynik = EXIT_FAILURE;
    } else if (strcmp(argv[1], "lista") == 0) {
        if (argc > 2) {
            fprintf(stderr, "Komenda 'lista' nie przyjmuje dodatkowych argumentów\n");
            fprintf(stderr, "Użyj: %s lista\n", argv[0]);
            wynik = EXIT_FAILURE;
        } else {
            if (klient_lista_zadan() != 0)
                wynik = EXIT_FAILURE;
        }
    } else if (strcmp(argv[1], "pomoc") == 0) {
        if (argc > 2) {
            fprintf(stderr, "Komenda 'pomoc' nie przyjmuje dodatkowych argumentów\n");
            fprintf(stderr, "Użyj: %s pomoc\n", argv[0]);
        } else {
            wypisz_komendy(argv[0]);
            wynik = EXIT_SUCCESS;
        }
    } else if (strcmp(argv[1], "anuluj") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Brak ID zadania do anulowania\n");
            fprintf(stderr, "Użyj: %s anuluj <id_zadania>\n", argv[0]);
            wynik = EXIT_FAILURE;
        } else if (argc > 3) {
            fprintf(stderr, "Komenda 'anuluj' przyjmuje tylko jeden argument (ID zadania)\n");
            fprintf(stderr, "Użyj: %s anuluj <id_zadania>\n", argv[0]);
            wynik = EXIT_FAILURE;
        } else {
            if (klient_anulowanie_zadania(argv[2]) != 0)
                wynik = EXIT_FAILURE;
        }
    } else if (strcmp(argv[1], "wylacz") == 0) {
        if (argc > 2) {
            fprintf(stderr, "Komenda 'wylacz' nie przyjmuje dodatkowych argumentów\n");
            fprintf(stderr, "Użyj: %s wylacz\n", argv[0]);
            wynik = EXIT_FAILURE;
        } else {
            if (klient_zatrzymanie_serwera() != 0)
                wynik = EXIT_FAILURE;
        }
    } else {
        fprintf(stderr, "Nieznane polecenie: %s\n", argv[1]);
        wypisz_komendy(argv[0]);
        wynik = EXIT_FAILURE;
    }

    return wynik;
}
