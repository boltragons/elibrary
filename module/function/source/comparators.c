#include "comparators.h"

#include "book.h"
#include "user.h"
#include "borrow.h"

bool it_search_book_by_id(void *book, void *args) {
    Book list_book = (Book) book;
    size_t *book_id = (size_t *) args;

    return book_get_id(list_book) == *book_id;
}

bool it_search_borrow_by_id(void *borrow, void *args) {
    Borrow list_borrow = (Borrow) borrow;
    size_t *borrow_id = (size_t *) args;

    return borrow_get_id(list_borrow) == *borrow_id;
}

bool it_search_user_by_id(void *user, void *args) {
    User list_user = (User) user;
    size_t *user_id = (size_t *) args;

    return user_get_id(list_user) == *user_id;
}
