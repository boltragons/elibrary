#include "interface.h"

#include <stdio.h>
#include <string.h>

void ui_show_header(const String subtitle) {
    ui_clear_screen();
    printf(
        "\033[1;40m\t\t\t\t\t--------------\n"
        "\t\t\t\t\t|  \033[1;31meLibrary\033[0m\033[1;40m  |\n"
        "\t\t\t\t\t--------------\033[0m\n"       
    );
    if(subtitle) {
        printf("\n\033[1;40m%s\033[0m", subtitle);
    }
}

void ui_show_message(const String message) {
    printf("\n%s\n", message);
    ui_wait_enter();
}

void ui_show_success(const String message) {
    ui_show_header(message);
    printf("\n");
    ui_wait_enter();
}

void ui_show_error(const String message, system_error_t error) {
    static StringBuffer message_buffer;
    snprintf(
        message_buffer,
        sizeof(StringBuffer),
        "%s\n"
        "Contact the system administrator [error-%03d].",
        message,
        (int) error
    );

    ui_show_header("An error has occurred.");
    ui_show_message(message_buffer);
}

void ui_clear_screen(void) {
    printf("\033[H\033[J");
}

void ui_wait_enter(void) {
    printf("\nPress ENTER to continue...");
    while(getchar() != '\n');
}

bool ui_read(String buffer, String message) {
    ui_show_header(message);
        
    scanf("%[^\n]", buffer);
    common_clean_buffer();

    return (strlen(buffer) != 0);
}

bool ui_read_text_field(String buffer, String entity, String field, String default_action) {
    static StringBuffer message;

    snprintf(
        message,
        sizeof(StringBuffer),
        "Insert the %s %s (or empty to %s): ",
        entity,
        field,
        default_action
    );
    
    return ui_read(buffer, message);
}

bool ui_read_integer_field(size_t *integer, String entity, String field, String default_action) {
    static IntegerBuffer integer_buffer;

    bool success = true;
    do {
        if(!ui_read_text_field(integer_buffer, entity, field, default_action)) {
            return false;
        }

        success = common_parse_number(integer_buffer, integer);

        CLEAR_INTEGER_BUFFER(integer_buffer);

        if(!success) {
            ui_show_message("Invalid input! Please enter a valid number.");
        }
    } while(!success);

    return true;
}

bool ui_read_phone_field(String phone_buffer, String entity, String field, String default_action) {
    bool success = true;
    do {
        if(!ui_read_text_field(phone_buffer, entity, field, default_action)) {
            return false;
        }

        success = common_verify_phone(phone_buffer);

        if(!success) {
            ui_show_message("Invalid input! Please enter a valid phone number in the format +XX XX XXX XX XX.");
        }
    } while(!success);

    return true;
}

bool ui_read_date_field(String date_buffer, String entity, String field, String default_action) {
    bool success = true;
    do {
        if(!ui_read_text_field(date_buffer, entity, field, default_action)) {
            return false;
        }

        success = common_verify_date(date_buffer);

        if(!success) {
            ui_show_message("Invalid input! Please enter a valid date format XX/XX/XXXX.");
        }
    } while(!success);

    return true;
}
