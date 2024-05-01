#ifndef BORROW_H
#define BORROW_H

#include "common.h"

/* Data Types */

/* Forward Declaration */
struct borrow_t;

/* Object Definition */
typedef struct borrow_t* Borrow;

/* Public Functions */

Borrow borrow_new(size_t id, size_t user_id, size_t book_id, const String borrow_date, const String return_date);

size_t borrow_get_id(const Borrow borrow);

size_t borrow_get_user_id(const Borrow borrow);

size_t borrow_get_book_id(const Borrow borrow);

const String borrow_get_date(const Borrow borrow);

const String borrow_get_return_date(const Borrow borrow);

bool borrow_has_returned(const Borrow borrow);

bool borrow_set_date(Borrow borrow, const String borrow_date);

/**
 * Must be called only after borrow date have been setted. 
 */
bool borrow_set_return_date(Borrow borrow, const String return_date);

bool borrow_return(Borrow borrow);

void borrow_delete(Borrow borrow);

/* Public Wrappers */

void borrow_deleter(void *borrow);

#endif
