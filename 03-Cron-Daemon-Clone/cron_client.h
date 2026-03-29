#ifndef CRON_CLIENT_H
#define CRON_CLIENT_H

#include "MyCron.h"

void wypisz_komendy(const char *prog_name);

int klient_dodanie_zadania(typ_komunikatu_t typ, int argc, char *argv[]);

int klient_lista_zadan(void);

int klient_anulowanie_zadania(const char *id_str);

int klient_zatrzymanie_serwera(void);

#endif
