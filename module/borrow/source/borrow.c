#include "borrow.h"

#include <string.h>

/* Data Types Implementation */

typedef struct borrow_t {
    size_t id;
    size_t user_id;
    size_t book_id;
    Date date;
    Date return_date;
    bool returned;
} borrow_t;

/* Public Functions */

Borrow borrow_new(size_t id, size_t user_id, size_t book_id, const String date, const String return_date) {
    /* Create receptacle */
    borrow_t borrow = {
        .id = id,
        .user_id = user_id,
        .book_id = book_id,
        .returned = false
    };

    if(
        !borrow_set_date(&borrow, date) ||
        !borrow_set_return_date(&borrow, return_date)
    ) {
        return NULL;
    }

    /* If everything is OK, copy to object */
    borrow_t *borrow_obj = new(borrow_t);
    memcpy(borrow_obj, &borrow, sizeof(borrow_t));

    return borrow_obj;
}

size_t borrow_get_id(const Borrow borrow) {
    if(!borrow) {
        return 0;
    }
    return borrow->id;
}

size_t borrow_get_user_id(const Borrow borrow) {
    if(!borrow) {
        return 0;
    }
    return borrow->user_id;
}

size_t borrow_get_book_id(const Borrow borrow) {
    if(!borrow) {
        return 0;
    }
    return borrow->book_id;
}

const String borrow_get_date(const Borrow borrow) {
    if(!borrow) {
        return NULL;
    }
    return borrow->date;
}

const String borrow_get_return_date(const Borrow borrow) {
    if(!borrow) {
        return NULL;
    }
    return borrow->return_date;
}

bool borrow_has_returned(const Borrow borrow) {
    if(!borrow) {
        return false;
    }
    return borrow->returned;
}

bool borrow_set_date(Borrow borrow, const String date) {
    if(!borrow || !date || !common_verify_date(date)) {
        return false;
    }

    strcpy(borrow->date, date);
    return true;
}

bool borrow_set_return_date(Borrow borrow, const String return_date) {
    if(!borrow || !return_date || !common_verify_date(return_date) || common_compare_dates(return_date, borrow->date) != DATE_GREATER) {
        return false;
    }

    strcpy(borrow->return_date, return_date);
    return true;
}

bool borrow_return(Borrow borrow) {
    if(!borrow || borrow->returned == true) {
        return false;
    }
    borrow->returned = true;
    return true;
}

void borrow_delete(Borrow borrow) {
    free(borrow);
}

/* Public Wrappers */

void borrow_deleter(void *borrow) {
    borrow_delete((Borrow) borrow);
}
