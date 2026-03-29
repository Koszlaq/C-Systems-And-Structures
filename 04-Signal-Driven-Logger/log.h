#ifndef LOG_H
#define LOG_H

#include <stdlib.h>
#include <semaphore.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <stdatomic.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

#define SYGNAL_ZMIANY_POZIOMU SIGRTMIN
#define SYGNAL_LOGOWANIE (SIGRTMIN+1)
#define SYGNAL_DUMP (SIGRTMIN+2)

typedef enum { MIN, STANDARD, MAX } PoziomSzczegolowosci;

short logger_inicjalizacja(const char *nazwa);

void logger_czyszczenie(void);

void zapisz_log(const int poziom, const char *format, ...);

#endif
