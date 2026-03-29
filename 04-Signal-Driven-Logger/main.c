#include "log.h"

int licznik = 0;
double temp = 36.5;

int main() {
    srand(time(NULL));
    printf("PID programu: %d\n", getpid());

    if (logger_inicjalizacja("aplikacja.log") != EXIT_SUCCESS) {
        printf("Blad inicjalizacji loggera!\n");
        return 1;
    }

    printf("Sygnaly do testow:\n");
    printf("sigqueue %d %d 0  # MIN\n", getpid(), SIGRTMIN);
    printf("sigqueue %d %d 1  # STANDARD\n", getpid(), SIGRTMIN);
    printf("sigqueue %d %d 2  # MAX\n", getpid(), SIGRTMIN);
    printf("kill -%d %d  # toggle logowania\n", SIGRTMIN+1, getpid());
    printf("kill -%d %d  # dump\n", SIGRTMIN+2, getpid());


    for (int i=0; i<100;i++) {
        licznik++;
        temp += 0.1;

        zapisz_log(MIN, "Petla MIN licznik=%d", licznik);
        zapisz_log(STANDARD, "Petla STD temperatura=%.2f", temp);
        zapisz_log(MAX, "Petla MAX debug=%d", rand()%2000);

        sleep(2);
    }

    logger_czyszczenie();
    return 0;
}

