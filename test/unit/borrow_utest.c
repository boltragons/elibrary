#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "borrow.h"

int main() {
    /* Test Artifacts */
    Borrow borrow = NULL;

    /* TEST 00 - VERIFY DATE (USES common.h) */
    assert(common_verify_date("30/04/2024") == true);

    date_t date = common_date_fields("30/08/2024");
    assert(date.day == 30);
    assert(date.month == 8);
    assert(date.year == 2024);

    assert(common_verify_date("32/04/2024") == false);
    assert(common_verify_date("30/14/2024") == false);
    assert(common_verify_date("30/06/4024") == false);
    assert(common_verify_date("30-10-2020") == false);
    assert(common_compare_dates("23/04/2024", "08/10/2024") == DATE_LESS);
    assert(common_compare_dates("23/04/2024", "23/04/2024") == DATE_EQUAL);
    assert(common_compare_dates("23/04/2024", "08/04/2024") == DATE_GREATER);

    /* TEST 01 - CREATE BORROW */
    borrow = borrow_new(1, 2, 3, "23/04/2024", "08/10/2024");
    assert(borrow != NULL);

    /* TEST 02 - GETTERS */
    assert(borrow_get_id(borrow) == 1);
    assert(borrow_get_user_id(borrow) == 2);
    assert(borrow_get_book_id(borrow) == 3);
    assert(strcmp(borrow_get_date(borrow), "23/04/2024") == 0);
    assert(strcmp(borrow_get_return_date(borrow), "08/10/2024") == 0);

    /* TEST 03 - BORROW/RETURN */
    assert(borrow_has_returned(borrow) == false);
    assert(borrow_return(borrow) == true);
    assert(borrow_has_returned(borrow) == true);
    assert(borrow_return(borrow) == false);

    /* TEST 04 - SETTERS */
    assert(borrow_set_date(borrow, "30/04/2024") == true);
    assert(borrow_set_date(borrow, "32/04/2024") == false);
    assert(borrow_set_date(borrow, "30/14/2024") == false);
    assert(borrow_set_date(borrow, "30/06/4024") == false);
    assert(borrow_set_date(borrow, "30-10-2020") == false);
    assert(borrow_set_return_date(borrow, "29/06/2024") == true);
    assert(borrow_set_return_date(borrow, "03/04/2024") == false);

    /* TEST 05 - DELETE */
    borrow_delete(borrow); // DELETER(borrow);

    return 0;
}