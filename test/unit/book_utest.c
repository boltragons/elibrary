#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "book.h"

int main() {
    /* Test Artifacts */
    Book book = NULL;

    /* TEST 01 - CREATE BOOK */
    book = book_new(1, "Moby Dick", "Herman Melville", "Adventure", 3);
    assert(book != NULL);

    /* TEST 02 - GETTERS */
    assert(book_get_id(book) == 1);
    assert(strcmp(book_get_title(book), "Moby Dick") == 0);
    assert(strcmp(book_get_author(book), "Herman Melville") == 0);
    assert(strcmp(book_get_genre(book), "Adventure") == 0);
    assert(book_get_quantity(book) == 3);
    assert(book_get_available(book) == 3);

    /* TEST 03 - BORROW/RETURN */
    assert(book_has_available(book) == true);
    assert(book_borrow(book) == true);
    assert(book_borrow(book) == true);
    assert(book_borrow(book) == true);
    assert(book_borrow(book) == false);
    assert(book_has_available(book) == false);
    assert(book_return(book) == true);
    assert(book_has_available(book) == true);
    assert(book_return(book) == true);
    assert(book_return(book) == true);
    assert(book_return(book) == false);

    /* TEST 04 - SETTERS */
    assert(book_set_title(book, "MOBY DICK") == true);
    assert(book_set_title(book, "") == false);
    assert(book_set_author(book, "H. M.") == true);
    assert(book_set_author(book, "") == false);
    assert(book_set_genre(book, "ADV.") == true);
    assert(book_set_genre(book, "") == false);
    assert(book_set_quantity(book, 6) == true);
    assert(book_set_available(book, 2) == true);
    assert(book_set_available(book, 8) == false);

    /* TEST 05 - DELETE */
    book_delete(book);

    return 0;
}