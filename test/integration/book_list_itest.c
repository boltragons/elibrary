#include <stdio.h>
#include <assert.h>

#include "list.h"
#include "book.h"
#include "serializer.h"

void printer(void *content, void *args) {
    Book book = (Book) content;
    printf(
        "Title: %s\n",
        book_get_title(book)
    );
}

bool comparer_by_quantity(void *book01, void *book02) {
    Book book_obj_01 = (Book) book01;
    Book book_obj_02 = (Book) book02;

    return book_get_quantity(book_obj_01) > book_get_quantity(book_obj_02);
}

int main() {
    /* Test Artifacts */
    List book_list = list_new();
    Book book = NULL;

    /* List Elements */
    Book book01 = book_new(1, "Moby Dick", "Herman Melville", "Adventure", 13);
    Book book02 = book_new(2, "Le Petit Prince", "Someone", "Romance", 32);
    Book book03 = book_new(3, "The Bible", "Everyone", "Biblical", 50);
    Book book04 = book_new(4, "World History", "Whoever", "History", 7);
    Book book05 = book_new(5, "How to Draw", "Me", "Educational", 2);

    /* TEST 01 - PUSH BACK INITIAL ELEMENTS */
    assert(list_push_back(book_list, book01) == book01);
    assert(list_push_back(book_list, book02) == book02);
    assert(list_push_back(book_list, book03) == book03);
    assert(list_push_back(book_list, book04) == book04);

    /* TEST 02 - PRINT INITIAL ELEMENTS */
    printf("\nFOREACH #1\n");
    list_foreach(book_list, &printer, NULL);

    /* TEST 04 - GET ELEMENT */
    book = (Book) list_get(book_list, 2);
    assert(book == (Book) book03);

    /* TEST 05 - REMOVE ELEMENT */
    book = (Book) list_remove(book_list, 2);
    assert(book == (Book) book03);
    book_delete(book);

    /* TEST 06 - POP BACK */
    book = (Book) list_pop_back(book_list);
    assert(book == (Book) book04);
    book_delete(book);

    /* TEST 07 - PUSH AFTER POP */
    assert(list_push_back(book_list, book05) == book05);

    /* TEST 08 - PRINT ELEMENTS AFTER VARIOUS OPERATIONS */
    printf("\nFOREACH #2\n");
    list_foreach(book_list, &printer, NULL);

    /* TEST 09 - GET FIRST ELEMENT */
    book = (Book) list_get(book_list, 0);
    assert(book == (Book) book01);
    assert(list_get(book_list, 0) == list_front(book_list));

    /* TEST 10 - GET LAST ELEMENT */
    book = (Book) list_get(book_list, list_size(book_list) - 1);
    assert(book == (Book) book05);
    assert(list_get(book_list, list_size(book_list) - 1) == list_back(book_list));

    /* TEST 11 - LIST SIZE */
    assert(list_size(book_list) == 3);

    /* TEST 12 - SAVE LIST */
    assert(serializer_save_books("test_books.csv", book_list, 6) == true);

    /* TEST 13 - FREE LIST */
    list_delete(book_list, &book_deleter);

    /* TEST 14 - RELOAD LIST */
    book_list = list_new();
    assert(serializer_load_books("test_books.csv", book_list) == 6);

    /* TEST 15 - SORT AND PRINT ELEMENTS AFTER RELOAD */
    assert(list_sort(book_list, &comparer_by_quantity) == true);
    printf("\nFOREACH #3\n");
    list_foreach(book_list, &printer, NULL);

    /* TEST 16 - FREE RELOADED LIST */
    list_delete(book_list, &book_deleter);
    
    return 0;
}
