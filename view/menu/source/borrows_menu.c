#include "borrows_menu.h"

#include <stdio.h>
#include <string.h>

#include "book.h"
#include "user.h"
#include "borrow.h"
#include "interface.h"
#include "serializer.h"
#include "comparators.h"

/* Private Iteration Functions */

static void it_print_borrow(void *borrow, void *args) {
    Borrow borrow_obj = (Borrow) borrow;
    printf(
        "%-10lu%-10lu%-10lu%-15s%-15s%-20s\n", 
        borrow_get_id(borrow_obj),
        borrow_get_book_id(borrow_obj),
        borrow_get_user_id(borrow_obj),
        borrow_get_date(borrow_obj),
        borrow_get_return_date(borrow_obj),
        borrow_has_returned(borrow_obj)? "Returned" : "Not returned"
    );
}

/* Private Menu Functions */

static void ui_show_borrows_add_menu(List borrow_list, List book_list, List user_list, size_t *borrow_last_id, size_t *book_last_id, size_t *user_last_id) {
    /* Function Preambule */
    static StringBuffer buffers[2];
    static size_t book_id_buffer;
    static size_t user_id_buffer;

    Book book = NULL;

    /* Obtain the Book ID */
    while(true) {
        if(!READ_INTEGER_FIELD_DEF_CLOSE(&book_id_buffer, "borrow", "book ID")) {
            return;
        }

        book = list_search(book_list, it_search_book_by_id, &book_id_buffer);

        if(!book) {
            ui_show_message("There is no book with the given ID.");
            continue;
        }
        
        if(book_has_available(book)) {
            break;
        } 

        ui_show_message("The requested book has no copies available.");
    };

    /* Obtain the User ID */
    while(true) {
        if(!READ_INTEGER_FIELD_DEF_CLOSE(&user_id_buffer, "borrow", "user ID")) {
            return;
        }
        if(list_search(user_list, &it_search_user_by_id, &user_id_buffer)) {
            break;
        }
        
        ui_show_message("There is no user with the given ID.");
    };

    /* Obtain the Borrow Date */
    if(!READ_DATE_FIELD_DEF_CLOSE(buffers[0], "borrow", "date")) {
        return;
    }

    /* Obtain the Return Date */
    while(true) {
        if(!READ_DATE_FIELD_DEF_CLOSE(buffers[1], "borrow", "return date")) {
            CLEAR_STRING_BUFFER(buffers[0]);
            return;
        }
        if(common_compare_dates(buffers[1], buffers[0]) == DATE_GREATER) {
            break;
        }
        
        ui_show_message("The return date must be after the borrow date.");
    };

    Borrow borrow = borrow_new(
        *borrow_last_id,
        user_id_buffer,
        book_id_buffer,
        buffers[0],
        buffers[1]
    );

    if(
        borrow &&
        list_push_back(borrow_list, borrow) &&
        book_borrow(book) &&
        serializer_save_borrows(BORROWS_FILE, borrow_list, ++(*borrow_last_id))
    ) {
        ui_show_success("Borrow added successfully!");
    }
    else {
        ui_show_error(
            "There was an error adding the borrow to the system",
            SYSTEM_ERROR_BORROW_CREATE
        );
    }

    CLEAR_STRING_BUFFER_ARRAY(buffers, 2);
}

static void ui_show_borrows_list_menu(List borrow_list, List book_list, List user_list, size_t *borrow_last_id, size_t *book_last_id, size_t *user_last_id) {
    ui_show_header(NULL);

    if(list_empty(borrow_list)) {
        ui_show_message("There are no borrows.");
        return;
    }

    printf(
        "\n\033[1;40m%-10s%-10s%-10s%-15s%-15s%-20s\033[0m\n",
        "ID",
        "Book ID",
        "User ID",
        "Date",
        "Return Date",
        "Status"
    );
    list_foreach(borrow_list, &it_print_borrow, NULL);

    ui_wait_enter();
}

static void ui_show_borrows_update_menu(List borrow_list, List book_list, List user_list, size_t *borrow_last_id, size_t *book_last_id, size_t *user_last_id) {
    /* Function Preambule */
    static StringBuffer buffer;
    static size_t id_buffer;
    
    bool some_field_modified = false;

    if(!READ_INTEGER_FIELD_DEF_CLOSE(&id_buffer, "borrow", "ID")) {
        return;
    }

    Borrow borrow = (Borrow) list_search(
        borrow_list, 
        &it_search_borrow_by_id, 
        &id_buffer
    );

    if(!borrow) {
        ui_show_message("Borrow not found!");
        return;
    }
    else if(borrow_has_returned(borrow)) {
        ui_show_message("The borrow has already been resolved!");
        return;
    }

    ui_show_header("Do you want to return the book? (y/n): ");
    char option = getchar();
    common_clean_buffer();

    if(option == 'y') {
        size_t book_id = borrow_get_book_id(borrow);

        Book book = (Book) list_search(
            book_list, 
            &it_search_book_by_id, 
            &book_id
        );

        if(!book) {
            ui_show_error(
                "There was an error while updating the borrow. The book must have been deleted.",
                SYSTEM_ERROR_BORROW_UPDATE
            );
            return;
        }

        book_return(book);
        borrow_return(borrow);
        some_field_modified = true;
    }
    else  {
        if(READ_DATE_FIELD_DEF_CONTINUE(buffer, "borrow", "new return date to renew")) {
            borrow_set_return_date(borrow, buffer);
            CLEAR_STRING_BUFFER(buffer);
            some_field_modified = true;
        }
    }

    if(!some_field_modified) {
        return;
    }

    if(
        serializer_save_borrows(BORROWS_FILE, borrow_list, *borrow_last_id) &&
        serializer_save_books(BOOKS_FILE, book_list, *book_last_id)
    ) {
        ui_show_success("Borrow updated successfully!");
    }
    else {
        ui_show_error(
            "There was an error updating the borrow",
            SYSTEM_ERROR_BORROW_UPDATE
        );
    }
}

/* Public Functions */

void ui_show_borrows_menu(List borrow_list, List book_list, List user_list, size_t *borrow_last_id, size_t *book_last_id, size_t *user_last_id) {
    int option = -1;

    do {
        ui_show_header(
            "1) Add borrow\n"
            "2) List borrows\n"
            "3) Update borrow\n"
            "0) Return\n"
            "> "
        );

        scanf("%d", &option);
        common_clean_buffer();

        switch (option) {
            case 0:
                break;
            case 1:
                ui_show_borrows_add_menu(borrow_list, book_list, user_list, borrow_last_id, book_last_id, user_last_id);
                break;
            case 2:
                ui_show_borrows_list_menu(borrow_list, book_list, user_list, borrow_last_id, book_last_id, user_last_id);
                break;
            case 3:
                ui_show_borrows_update_menu(borrow_list, book_list, user_list, borrow_last_id, book_last_id, user_last_id);
                break;
            default:
                ui_show_message("Invalid option! Try again.");
        }
    } while (option != 0);
}
