#ifndef INTERFACE_H
#define INTERFACE_H

#include "common.h"

/* Data Types */

typedef enum system_error_t {
    NO_SYSTEM_ERROR,
    SYSTEM_ERROR_DATABASE_LOAD,
    SYSTEM_ERROR_DATABASE_SAVE,
    SYSTEM_ERROR_BOOK_CREATE,
    SYSTEM_ERROR_BOOK_LIST,
    SYSTEM_ERROR_BOOK_UPDATE,
    SYSTEM_ERROR_BOOK_DELETE,
    SYSTEM_ERROR_USER_CREATE,
    SYSTEM_ERROR_USER_LIST,
    SYSTEM_ERROR_USER_UPDATE,
    SYSTEM_ERROR_USER_DELETE,
    SYSTEM_ERROR_BORROW_CREATE,
    SYSTEM_ERROR_BORROW_LIST,
    SYSTEM_ERROR_BORROW_UPDATE,
    SYSTEM_ERROR_BORROW_DELETE
} system_error_t;

/* Public Macros */

#define READ_TEXT_FIELD_DEF_CLOSE(b, e, f)          ui_read_text_field(b, e, f, "cancel")
#define READ_TEXT_FIELD_DEF_CONTINUE(b, e, f)       ui_read_text_field(b, e, f, "continue")

#define READ_INTEGER_FIELD_DEF_CLOSE(b, e, f)       ui_read_integer_field(b, e, f, "cancel")
#define READ_INTEGER_FIELD_DEF_CONTINUE(b, e, f)    ui_read_integer_field(b, e, f, "continue")

#define READ_PHONE_FIELD_DEF_CLOSE(b, e, f)         ui_read_phone_field(b, e, f, "cancel")
#define READ_PHONE_FIELD_DEF_CONTINUE(b, e, f)      ui_read_phone_field(b, e, f, "continue")

#define READ_DATE_FIELD_DEF_CLOSE(b, e, f)          ui_read_date_field(b, e, f, "cancel")
#define READ_DATE_FIELD_DEF_CONTINUE(b, e, f)       ui_read_date_field(b, e, f, "continue")

/* Public Functions */

void ui_show_header(const String subtitle);

void ui_show_message(const String message);

void ui_show_success(const String message);

void ui_show_error(const String message, system_error_t error);

void ui_clear_screen(void);

void ui_wait_enter(void);

bool ui_read(String buffer, String message);

bool ui_read_text_field(String buffer, String entity, String field, String default_action);

bool ui_read_integer_field(size_t *integer, String entity, String field, String default_action);

bool ui_read_phone_field(String phone_buffer, String entity, String field, String default_action);

bool ui_read_date_field(String date_buffer, String entity, String field, String default_action);

#endif
