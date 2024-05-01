#ifndef LOGGER_H
#define LOGGER_H

#define LOGS_FILE "log.txt"

#ifdef DEBUG
#include <stdio.h>

#define LOG(f, ...)\
    do {\
    FILE *log_file = fopen(LOGS_FILE, "w");\
    fprintf(log_file, "[%s:%d] "f"\n", __FILE__, __LINE__, ##__VA_ARGS__);\
    fclose(log_file);\
    } while(0)
#else
#define LOG(f, ...) ;
#endif

#endif
