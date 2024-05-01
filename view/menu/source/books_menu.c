#include "books_menu.h"

#include <stdio.h>
#include <string.h>

#include "book.h"
#include "interface.h"
#include "serializer.h"
#include "comparators.h"

/* Private Data Types */

typedef struct print_control_t {
    String filter;
    size_t number_results;
} print_control_t;

/* Private Iteration Functions */

static void it_print_book(void *book, void *args) {
    Book book_obj = (Book) book;
    print_control_t *print_control = (print_control_t *) args;

    if(
        print_control->filter &&
        strcmp(book_get_title(book_obj), print_control->filter) != 0 &&
        strcmp(book_get_author(book_obj), print_control->filter) != 0 &&
        strcmp(book_get_genre(book_obj), print_control->filter) != 0
    ) {
        return;
    }

    print_control->number_results++;

    printf(
        "%-8lu%-50s%-30s%-20s%-10lu%-10lu\n", 
        book_get_id(book_obj),
        book_get_title(book_obj),
        book_get_author(book_obj),
        book_get_genre(book_obj),
        book_get_quantity(book_obj),
        book_get_available(book_obj)
    );
}

/* Private Menu Functions */

static void ui_show_books_add_menu(List book_list, size_t *book_last_id) {
    /* Function Preambule */
    static StringBuffer buffers[3];
    static size_t quantity_buffer;

    if(
        READ_TEXT_FIELD_DEF_CLOSE(buffers[0], "book", "title")    &&
        READ_TEXT_FIELD_DEF_CLOSE(buffers[1], "book", "author")   &&
        READ_TEXT_FIELD_DEF_CLOSE(buffers[2], "book", "genre")    &&
        READ_INTEGER_FIELD_DEF_CLOSE(&quantity_buffer, "book", "quantity")
    ) {
        Book book = book_new(
            *book_last_id,
            buffers[0],
            buffers[1],
            buffers[2],
            quantity_buffer
        );

        if(
            book &&
            list_push_back(book_list, book) &&
            serializer_save_books(BOOKS_FILE, book_list, ++(*book_last_id))
        ) {
            ui_show_success("Book added successfully!");
        }
        else {
            ui_show_error(
                "There was an error adding the book to the system",
                SYSTEM_ERROR_BOOK_CREATE
            );
        }
    }

    CLEAR_STRING_BUFFER_ARRAY(buffers, 3);
}

static void ui_show_books_list_menu(List book_list, size_t *book_last_id) {
    /* Function Preambule */
    static print_control_t print_control;
    static StringBuffer buffer;
    CLEAR_STRING_BUFFER(buffer);
    print_control.number_results = 0;

    ui_show_header(NULL);

    if(list_empty(book_list)) {
        ui_show_message("There are no books.");
        return;
    }

    print_control.filter = 
        ui_read_text_field(
            buffer, 
            "book", 
            "title, author or genre", 
            "show all"
        ) ? buffer : NULL;
    
    printf(
        "\n\033[1;40m%-8s%-50s%-30s%-20s%-10s%-10s\033[0m\n",
        "ID",
        "Title",
        "Author",
        "Genre",
        "Quantity",
        "Available"
    );
    list_foreach(book_list, &it_print_book, &print_control);

    if(print_control.number_results == 0) {
        ui_show_header(NULL);
        ui_show_message("No results matches the filter.");
    }
    else {
        ui_wait_enter();
    }
}

static void ui_show_books_update_menu(List book_list, size_t *book_last_id) {
    /* Function Preambule */
    static StringBuffer buffers[3];
    static size_t id_buffer;
    static size_t quantity_buffer;
    
    bool some_field_modified = false;

    if(!READ_INTEGER_FIELD_DEF_CLOSE(&id_buffer, "book", "ID")) {
        return;
    }

    Book book = (Book) list_search(book_list, &it_search_book_by_id, &id_buffer);

    if(!book) {
        ui_show_message("Book not found!");
        return;
    }

    if(READ_TEXT_FIELD_DEF_CONTINUE(buffers[0], "book", "title")) {
        book_set_title(book, buffers[0]);
        CLEAR_STRING_BUFFER(buffers[0]);
        some_field_modified = true;
    }

    if(READ_TEXT_FIELD_DEF_CONTINUE(buffers[1], "book", "author")) {
        book_set_author(book, buffers[1]);
        CLEAR_STRING_BUFFER(buffers[1]);
        some_field_modified = true;
    }

    if(READ_TEXT_FIELD_DEF_CONTINUE(buffers[2], "book", "genre")) {
        book_set_genre(book, buffers[2]);
        CLEAR_STRING_BUFFER(buffers[2]);
        some_field_modified = true;
    }

    while(true) {
        if(!READ_INTEGER_FIELD_DEF_CONTINUE(&quantity_buffer, "book", "quantity")) {
            break;
        }

        if(quantity_buffer >= book_get_quantity(book) - book_get_available(book)) {
            book_set_quantity(book, quantity_buffer);
            some_field_modified = true;
            break;
        }

        ui_show_message("The number of books must be greater or equal to the borrowed quantity!");
    }

    if(!some_field_modified) {
        return;
    }

    if(
        serializer_save_books(BOOKS_FILE, book_list, *book_last_id)
    ) {
        ui_show_success("Book updated successfully!");
    }
    else {
        ui_show_error(
            "There was an error updating the book",
            SYSTEM_ERROR_BOOK_UPDATE
        );
    }
}

static void ui_show_books_delete_menu(List book_list, size_t *book_last_id) {
    /* Function Preambule */
    static size_t id_buffer;

    if(!READ_INTEGER_FIELD_DEF_CONTINUE(&id_buffer, "book", "ID")) {
        return;
    }

    if(
        !list_search_remove(book_list, &it_search_book_by_id, &id_buffer, &book_deleter)
    ) {
        ui_show_message("Book not found!");
        return;
    }

    if(
        serializer_save_books(BOOKS_FILE, book_list, *book_last_id)
    ) {
        ui_show_success("Book deleted successfully!");
    }
    else {
        ui_show_error(
            "There was an error deleting the book",
            SYSTEM_ERROR_BOOK_DELETE
        );
    }
}

/* Public Functions */

void ui_show_books_menu(List book_list, size_t *book_last_id) {
    int option = -1;

    do {
        ui_show_header(
            "1) Add book\n"
            "2) List books\n"
            "3) Update book\n"
            "4) Delete book\n"
            "0) Return\n"
            "> "
        );

        scanf("%d", &option);
        common_clean_buffer();

        switch (option) {
            case 0:
                break;
            case 1:
                ui_show_books_add_menu(book_list, book_last_id);
                break;
            case 2:
                ui_show_books_list_menu(book_list, book_last_id);
                break;
            case 3:
                ui_show_books_update_menu(book_list, book_last_id);
                break;
            case 4:
                ui_show_books_delete_menu(book_list, book_last_id);
                break;
            default:
                ui_show_message("Invalid option! Try again.");
        }
    } while (option != 0);
}
