#include "book.h"

#include <string.h>

#include "strman.h"

/* Private Macro */

#define AVAILABLE(book) (book->available != 0)

/* Data Types Implementation */

typedef struct book_t {
    size_t id;
    String title;
    String author;
    String genre;
    size_t quantity;
    size_t available;
} book_t;

/* Public Functions */

Book book_new(size_t id, const String title, const String author, const String genre, size_t quantity) {
    /* Create receptacle */
    book_t book = {
        .id = id,
        .title = NULL,
        .author = NULL,
        .genre = NULL,
        .quantity = quantity,
        .available = quantity
    };

    if(
        !book_set_title(&book, title) ||
        !book_set_author(&book, author) ||
        !book_set_genre(&book, genre)
    ) {
        return NULL;
    }

    /* If everything is OK, copy to object */
    book_t *book_obj = new(book_t);
    memcpy(book_obj, &book, sizeof(book_t));

    return book_obj;
}

void book_delete(Book book) {
    free(book->title);
    free(book->author);
    free(book->genre);

    free(book);
}

size_t book_get_id(const Book book) {
    return book->id;
}

String book_get_title(const Book book) {
    return book->title;
}

String book_get_author(const Book book) {
    return book->author;
}

String book_get_genre(const Book book) {
    return book->genre;
}

size_t book_get_quantity(const Book book) {
    return book->quantity;
}

size_t book_get_available(const Book book) {
    return book->available;
}

bool book_set_title(Book book, const String title) {
    if(!book || !title || strlen(title) == 0) {
        return false;
    }
    
    strman_copy(&book->title, title);
    return true;
}

bool book_set_author(Book book, const String author) {
    if(!book || !author || strlen(author) == 0) {
        return false;
    }

    strman_copy(&book->author, author);
    return true;
}

bool book_set_genre(Book book, const String genre) {
    if(!book || !genre || strlen(genre) == 0) {
        return false;
    }

    strman_copy(&book->genre, genre);
    return true;
}

bool book_set_quantity(Book book, size_t quantity) {
    if(!book) {
        return false;
    }

    book->quantity = quantity;
    return true;
}

bool book_set_available(const Book book, size_t available) {
    if(!book || book->quantity < available) {
        return false;
    }

    book->available = available;
    return true;
}

bool book_borrow(Book book) {
    if(!book || !AVAILABLE(book)) {
        return false;
    }

    book->available--;
    return true;
}

bool book_return(Book book) {
    if(!book || book->available == book->quantity) {
        return false;
    }

    book->available++;
    return true;
}

bool book_has_available(Book book) {
    return AVAILABLE(book);
}

/* Public Wrappers */

void book_deleter(void *book) {
    book_delete((Book) book);
}
