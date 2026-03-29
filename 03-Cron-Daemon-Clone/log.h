#ifndef LOG_H
#define LOG_H

#include "MyCron.h"
#include <semaphore.h>
#include <stdatomic.h>
#include <stdarg.h>

#define SYGNAL_ZMIANY_POZIOMU SIGRTMIN
#define SYGNAL_LOGOWANIE (SIGRTMIN+1)
#define SYGNAL_DUMP (SIGRTMIN+2)

typedef enum { MIN, STANDARD, MAX } PoziomSzczegolowosci;

short logger_inicjalizacja(const char *nazwa);

void logger_czyszczenie(void);

void zapisz_log(const int poziom, const char *format, ...);

#endif
