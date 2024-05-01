#include "common.h"

#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>

/* Private Macros */

#define PHONE_REGEX "^[+]{1}[0-9]{2} [0-9]{2} [0-9]{3} [0-9]{2} [0-9]{2}$"
#define DATE_REGEX  "^[0-9]{2}[/]{1}[0-9]{2}[/]{1}[0-9]{4}$"

/* Public Functions */

bool common_verify_phone(const String phone) {
    if(!phone) {
        return false;
    }
    
    regex_t regex;
    regcomp(&regex, PHONE_REGEX, REG_EXTENDED);
    int result =  regexec(&regex, phone, 0, NULL, 0);
    regfree(&regex);
    return result == REG_NOERROR;
}

bool common_verify_date(const String date) {
    if(!date) {
        return false;
    }
    
    regex_t regex;
    regcomp(&regex, DATE_REGEX, REG_EXTENDED);
    int result =  regexec(&regex, date, 0, NULL, 0);
    regfree(&regex);

    if(result != REG_NOERROR) {
        return false;
    }

    date_t date_fields = common_date_fields(date);

    return date_fields.day <= 31 &&
        date_fields.month <= 12 &&
        date_fields.year <= 2999;
}

int common_compare_dates(const String date01, const String date02) {
    if(!date01 || !date02) {
        return DATE_LESS;
    }
    date_t date01_fields = common_date_fields(date01);
    date_t date02_fields = common_date_fields(date02);

    if(date01_fields.year > date02_fields.year) {
        return DATE_GREATER;
    }
    else if(date01_fields.year < date02_fields.year) {
        return DATE_LESS;
    }
    else if(date01_fields.month > date02_fields.month) {
        return DATE_GREATER;
    }
    else if(date01_fields.month < date02_fields.month) {
        return DATE_LESS;
    }
    else if(date01_fields.day > date02_fields.day) {
        return DATE_GREATER;
    }
    else if(date01_fields.day < date02_fields.day) {
        return DATE_LESS;
    }
    else {
        return DATE_EQUAL;
    }
}

date_t common_date_fields(const String date) {
    date_t date_fields = {0};
    if(date) {
        sscanf(
            date,
            "%u/%u/%u", 
            &date_fields.day, 
            &date_fields.month, 
            &date_fields.year
        );
    }
    return date_fields;
}

bool common_is_number(const String str) {
    for(size_t i = 0; str[i] != '\0'; i++) {
        if(!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

bool common_parse_number(const String str, size_t *number) {
    if(!common_is_number(str)) {
        return false;
    }
    *number = strtol(str, NULL, 10);
    return true;
}

void common_clean_buffer(void) {
    while(getchar() != '\n');
}
