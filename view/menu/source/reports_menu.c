#include "reports_menu.h"

#include <stdio.h>
#include <string.h>

#include "book.h"
#include "user.h"
#include "borrow.h"
#include "interface.h"
#include "serializer.h"
#include "comparators.h"

/* Private Data Types */

typedef struct report_t {
    size_t id;
    size_t count;
} report_t;

/* Private Functions */

static inline report_t *report_new(size_t id) {
    report_t *report = new(report_t);
    report->id = id;
    report->count = 0;
    return report;
}

/* Private Iteration Functions (General) */

static bool it_report_comparer(void *report01, void *report02) {
    report_t *report_obj_01 = (report_t *) report01;
    report_t *report_obj_02 = (report_t *) report02;
    
    return report_obj_01->count < report_obj_02->count;
}

/* Private Iteration Functions (Most Borrowed Report) */

static void it_count_book_borrows(void *borrow, void *args) {
    Borrow borrow_obj = (Borrow) borrow;
    report_t *report = (report_t*) args;
    
    if(borrow_get_book_id(borrow_obj) == report->id) {
        report->count++;
    }
}

static void it_fill_most_borrowed_report(void *book, void *args) {
    Book book_obj = (Book) book;
    List *lists = (List*) args;
    List report_list = lists[0];
    List borrow_list = lists[1];

    report_t *report = report_new(book_get_id(book_obj));

    list_push_back(report_list, report);

    list_foreach(borrow_list, &it_count_book_borrows, report);
}

static void it_print_most_borrowed_report(void *report, void *args) {
    report_t *report_obj = (report_t*) report;
    List book_list = (List) args;
    
    if(report_obj->count == 0) {
        return;
    }

    Book book = list_search(
        book_list, 
        &it_search_book_by_id, 
        &report_obj->id
    );

    printf(
        "%-8lu%-50s%-30s%-20s%-10lu%-10lu%-10lu\n", 
        report_obj->id,
        book_get_title(book),
        book_get_author(book),
        book_get_genre(book),
        book_get_quantity(book),
        book_get_available(book),
        report_obj->count
    );
}

/* Private Iteration Functions (Non Returned Report) */

static void it_fill_non_returned_report(void *book, void *args) {
    Book book_obj = (Book) book;
    List report_list = (List) args;

    size_t non_returned = book_get_quantity(book_obj) - book_get_available(book_obj);
    if(non_returned == 0) {
        return;
    }

    report_t *report = report_new(book_get_id(book_obj));
    report->count = non_returned;

    list_push_back(report_list, report);
}

static void it_print_non_returned_report(void *report, void *args) {
    report_t *report_obj = (report_t*) report;
    List book_list = (List) args;
    
    if(report_obj->count == 0) {
        return;
    }

    Book book = list_search(
        book_list, 
        &it_search_book_by_id, 
        &report_obj->id
    );

    printf(
        "%-8lu%-50s%-30s%-20s%-10lu%-10lu\n", 
        report_obj->id,
        book_get_title(book),
        book_get_author(book),
        book_get_genre(book),
        book_get_quantity(book),
        report_obj->count
    );
}

/* Private Iteration Functions (User Borrows Report) */

static void it_count_user_borrows(void *borrow, void *args) {
    Borrow borrow_obj = (Borrow) borrow;
    report_t *report = (report_t*) args;
    
    if(borrow_get_user_id(borrow_obj) == report->id) {
        report->count++;
    }
}

static void it_fill_user_borrows_report(void *user, void *args) {
    User user_obj = (User) user;
    List *lists = (List*) args;
    List report_list = lists[0];
    List borrow_list = lists[1];

    report_t *report = report_new(user_get_id(user_obj));

    list_push_back(report_list, report);

    list_foreach(borrow_list, &it_count_user_borrows, report);
}

static void it_print_user_borrows_report(void *report, void *args) {
    report_t *report_obj = (report_t*) report;
    List user_list = (List) args;
    
    if(report_obj->count == 0) {
        return;
    }

    User user = list_search(
        user_list, 
        &it_search_user_by_id, 
        &report_obj->id
    );

    printf(
        "%-8lu%-50s%-50s%-20s%-10lu\n", 
        report_obj->id,
        user_get_name(user),
        user_get_address(user),
        user_get_phone(user),
        report_obj->count
    );
}

/* Private Menu Functions */

static void ui_show_most_borrowed_report(List borrow_list, List book_list) {
    /* Check for NO BORROWS and NO BOOKS case */
    ui_show_header(NULL);
    if(list_size(borrow_list) == 0) {
        ui_show_message("There are no borrows.");
        return;
    }
    else if(list_size(book_list) == 0) {
        ui_show_message("There are no books.");
        return;
    }

    List report_list = list_new();

    /* For each book, fill it's borrow report. */
    List lists[2] = {report_list, borrow_list};
    list_foreach(book_list, &it_fill_most_borrowed_report, lists);

    list_sort(report_list, &it_report_comparer);

    printf(
        "\n\033[1;40m%-8s%-50s%-30s%-20s%-10s%-10s%-10s\033[0m\n",
        "ID",
        "Title",
        "Author",
        "Genre",
        "Quantity",
        "Available",
        "Borrows"
    );
    
    list_foreach(report_list, &it_print_most_borrowed_report, book_list);

    ui_wait_enter();

    list_delete(report_list, &free);
}

static void ui_show_non_returned_report(List borrow_list, List book_list) {
    /* Check for NO BORROWS and NO BOOKS case */
    ui_show_header(NULL);
    if(list_size(borrow_list) == 0) {
        ui_show_message("There are no borrows.");
        return;
    }
    else if(list_size(book_list) == 0) {
        ui_show_message("There are no books.");
        return;
    }

    List report_list = list_new();

    /* For each book, fill it's non-return report. */
    list_foreach(book_list, &it_fill_non_returned_report, report_list);

    list_sort(report_list, &it_report_comparer);

    printf(
        "\n\033[1;40m%-8s%-50s%-30s%-20s%-10s%-10s\033[0m\n",
        "ID",
        "Title",
        "Author",
        "Genre",
        "Quantity",
        "Borrowed"
    );
    
    list_foreach(report_list, &it_print_non_returned_report, book_list);

    ui_wait_enter();

    list_delete(report_list, &free);
}

static void ui_show_user_borrows_report(List borrow_list, List user_list) {
    /* Check for NO BORROWS and NO USERS case */
    ui_show_header(NULL);
    if(list_size(borrow_list) == 0) {
        ui_show_message("There are no borrows.");
        return;
    }
    else if(list_size(user_list) == 0) {
        ui_show_message("There are no users.");
        return;
    }

    List report_list = list_new();

    /* For each user, fill it's borrow report. */
    List lists[2] = {report_list, borrow_list};
    list_foreach(user_list, &it_fill_user_borrows_report, lists);

    list_sort(report_list, &it_report_comparer);

    printf(
        "\n\033[1;40m%-8s%-50s%-50s%-20s%-10s\033[0m\n",
        "ID",
        "Name",
        "Address",
        "Phone",
        "Borrows"
    );
    
    list_foreach(report_list, &it_print_user_borrows_report, user_list);

    ui_wait_enter();

    list_delete(report_list, &free);
}

/* Public Functions */

void ui_show_reports_menu(List borrow_list, List book_list, List user_list) {
    int option = -1;

    do {
        ui_show_header(
            "1) Most borrowed books\n"
            "2) Non-returned books\n"
            "3) Users with more borrows\n"
            "0) Return\n"
            "> "
        );

        scanf("%d", &option);
        common_clean_buffer();

        switch (option) {
            case 0:
                break;
            case 1:
                ui_show_most_borrowed_report(borrow_list, book_list);
                break;
            case 2:
                ui_show_non_returned_report(borrow_list, book_list);
                break;
            case 3:
                ui_show_user_borrows_report(borrow_list, user_list);
                break;
            default:
                ui_show_message("Invalid option! Try again.");
        }
    } while (option != 0);
}
