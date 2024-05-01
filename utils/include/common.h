#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "logger.h"

/* Operators */

#define new(type)           ((type *) malloc(sizeof(type)))
#define new_block(type, n)  ((type *) malloc(n*sizeof(type)))

/* Function Pointers */

typedef void (*ConsumerFunction)(void*);
typedef void (*IterationFunction)(void*, void*);
typedef bool (*CompareFunction)(void*, void*);

/* Others Macros */

#define DELETER(obj)            (##obj##_deleter(obj))
#define INTEGER_BUFFER_SIZE     20
#define STRING_BUFFER_SIZE      256
#define OBJECT_STRING_SIZE      1024
#define DATE_SIZE               11
#define PHONE_SIZE              17
#define DATE_GREATER            1
#define DATE_EQUAL              0
#define DATE_LESS               -1
#define USERS_FILE               "users.csv"
#define BOOKS_FILE               "books.csv"
#define BORROWS_FILE             "borrows.csv"

#define CLEAR_BUFFER(str, bytes)            memset(str, 0, bytes)
#define CLEAR_STRING_BUFFER(str)            CLEAR_BUFFER(str, STRING_BUFFER_SIZE)
#define CLEAR_STRING_BUFFER_ARRAY(str, n)   CLEAR_BUFFER(str, n*STRING_BUFFER_SIZE)
#define CLEAR_INTEGER_BUFFER(str)           CLEAR_BUFFER(str, INTEGER_BUFFER_SIZE)
#define CLEAR_OBJECT_STRING(str)            CLEAR_BUFFER(str, OBJECT_STRING_SIZE)

/* Data types */

typedef unsigned int uint;

typedef char* String;

typedef char Date[DATE_SIZE]; 

typedef char Phone[PHONE_SIZE];

typedef char IntegerBuffer[INTEGER_BUFFER_SIZE]; 

typedef char StringBuffer[STRING_BUFFER_SIZE]; 

typedef char ObjectString[OBJECT_STRING_SIZE];

typedef struct date_t {
    uint day;
    uint month;
    uint year;
} date_t;

/* Public Functions */

bool common_verify_phone(const String phone);

bool common_verify_date(const String date);

int common_compare_dates(const String date01, const String date02);

date_t common_date_fields(const String date);

bool common_is_number(const String str);

bool common_parse_number(const String str, size_t *number);

void common_clean_buffer(void);

#endif
