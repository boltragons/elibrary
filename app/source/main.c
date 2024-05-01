#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "list.h"
#include "user.h"
#include "book.h"
#include "borrow.h"
#include "interface.h"
#include "serializer.h"
#include "books_menu.h"
#include "users_menu.h"
#include "borrows_menu.h"
#include "reports_menu.h"

/* Private Functions */

void app_show_main_menu(List user_list, List book_list, List borrow_list, size_t *user_last_id, size_t *book_last_id, size_t *borrow_last_id);

/* Main Function */

int main(void) {
    List user_list = list_new();
    List book_list = list_new();
    List borrow_list = list_new();

    size_t user_last_id = serializer_load_users(USERS_FILE, user_list);
    size_t book_last_id = serializer_load_books(BOOKS_FILE, book_list);
    size_t borrow_last_id = serializer_load_borrows(BORROWS_FILE, borrow_list);

    if(!user_last_id || !book_last_id || !borrow_last_id) {
        ui_show_error(
            "There was an error loading the database files.\n"
            "Contact the system administrator.",
            SYSTEM_ERROR_DATABASE_LOAD
        );
    }

    app_show_main_menu(user_list, book_list, borrow_list, &user_last_id, &book_last_id, &borrow_last_id);

    if(
        !serializer_save_users(USERS_FILE, user_list, user_last_id) ||
        !serializer_save_books(BOOKS_FILE, book_list, book_last_id) ||
        !serializer_save_borrows(BORROWS_FILE, borrow_list, borrow_last_id)
    ) {
        ui_show_error(
            "There was an error saving the database files.\n"
            "Contact the system administrator.",
            SYSTEM_ERROR_DATABASE_SAVE
        );
    }

    list_delete(user_list, &user_deleter);
    list_delete(book_list, &book_deleter);
    list_delete(borrow_list, &borrow_deleter);

    return EXIT_SUCCESS;
}

/* Private Functions */

void app_show_main_menu(List user_list, List book_list, List borrow_list, size_t *user_last_id, size_t *book_last_id, size_t *borrow_last_id) {
    int option = -1;

    do {
        ui_show_header(
            "1) Books Management\n"
            "2) Users Management\n"
            "3) Borrows Management\n"
            "4) Reports Viewer\n"
            "0) Exit\n"
            "> "
        );

        scanf("%d", &option);
        common_clean_buffer();

        switch (option) {
            case 0:
                break;
            case 1:
                ui_show_books_menu(book_list, book_last_id);
                break;
            case 2:
                ui_show_users_menu(user_list, user_last_id);
                break;
            case 3:
                ui_show_borrows_menu(borrow_list, book_list, user_list, borrow_last_id, book_last_id, user_last_id);
                break;
            case 4:
                ui_show_reports_menu(borrow_list, book_list, user_list);
                break;
            default:
                ui_show_message("Invalid option! Try again.");
        }
    } while (option != 0);

    ui_clear_screen();
}
