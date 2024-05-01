#ifndef BOOK_H
#define BOOK_H

#include "common.h"

/* Data Types */

/* Forward Declaration */
struct book_t;

/* Object Definition */
typedef struct book_t* Book;

/* Public Functions */

Book book_new(size_t id, const String title, const String author, const String genre, size_t quantity);

void book_delete(Book book);

size_t book_get_id(const Book book);

String book_get_title(const Book book);

String book_get_author(const Book book);

String book_get_genre(const Book book);

size_t book_get_quantity(const Book book);

size_t book_get_available(const Book book);

bool book_set_title(Book book, const String title);

bool book_set_author(Book book, const String author);

bool book_set_genre(Book book, const String genre);

bool book_set_quantity(Book book, size_t quantity);

bool book_set_available(Book book, size_t available);

bool book_borrow(Book book);

bool book_return(Book book);

bool book_has_available(const Book book);

/* Public Wrappers */

void book_deleter(void *book);

#endif
