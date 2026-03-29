#ifndef CRON_SERVER_H
#define CRON_SERVER_H

#include "MyCron.h"

#include <sys/stat.h>
#include <sys/file.h>
#include <spawn.h>
#include <pthread.h>

void petla_serwera(void);

int czy_serwer_dziala(void);

int uruchom_serwer(void);

#endif
