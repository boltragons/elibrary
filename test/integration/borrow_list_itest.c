#include <stdio.h>
#include <assert.h>

#include "list.h"
#include "borrow.h"
#include "serializer.h"

void printer(void *content, void *args) {
    Borrow borrow = (Borrow) content;
    printf(
        "Return Date: %s\n",
        borrow_get_return_date(borrow)
    );
}

int main() {
    /* Test Artifacts */
    List borrow_list = list_new();
    Borrow borrow = NULL;

    /* List Elements */
    Borrow borrow01 = borrow_new(1, 2, 3, "23/04/2024", "08/10/2024");
    Borrow borrow02 = borrow_new(2, 2, 5, "23/04/2024", "18/10/2024");
    Borrow borrow03 = borrow_new(3, 2, 1, "23/04/2024", "09/10/2024");
    Borrow borrow04 = borrow_new(4, 4, 2, "23/04/2024", "01/10/2024");
    Borrow borrow05 = borrow_new(5, 6, 1, "23/04/2024", "08/08/2024");

    /* TEST 01 - PUSH BACK INITIAL ELEMENTS */
    assert(list_push_back(borrow_list, borrow01) == borrow01);
    assert(list_push_back(borrow_list, borrow02) == borrow02);
    assert(list_push_back(borrow_list, borrow03) == borrow03);
    assert(list_push_back(borrow_list, borrow04) == borrow04);

    /* TEST 02 - PRINT INITIAL ELEMENTS */
    printf("\nFOREACH #1\n");
    list_foreach(borrow_list, &printer, NULL);

    /* TEST 04 - GET ELEMENT */
    borrow = (Borrow) list_get(borrow_list, 2);
    assert(borrow == (Borrow) borrow03);

    /* TEST 05 - REMOVE ELEMENT */
    borrow = (Borrow) list_remove(borrow_list, 2);
    assert(borrow == (Borrow) borrow03);
    borrow_delete(borrow);

    /* TEST 06 - POP BACK */
    borrow = (Borrow) list_pop_back(borrow_list);
    assert(borrow == (Borrow) borrow04);
    borrow_delete(borrow);

    /* TEST 07 - PUSH AFTER POP */
    assert(list_push_back(borrow_list, borrow05) == borrow05);

    /* TEST 08 - PRINT ELEMENTS AFTER VARIOUS OPERATIONS */
    printf("\nFOREACH #2\n");
    list_foreach(borrow_list, &printer, NULL);

    /* TEST 09 - GET FIRST ELEMENT */
    borrow = (Borrow) list_get(borrow_list, 0);
    assert(borrow == (Borrow) borrow01);
    assert(list_get(borrow_list, 0) == list_front(borrow_list));

    /* TEST 10 - GET LAST ELEMENT */
    borrow = (Borrow) list_get(borrow_list, list_size(borrow_list) - 1);
    assert(borrow == (Borrow) borrow05);
    assert(list_get(borrow_list, list_size(borrow_list) - 1) == list_back(borrow_list));

    /* TEST 11 - LIST SIZE */
    assert(list_size(borrow_list) == 3);

    /* TEST 12 - SAVE LIST */
    assert(serializer_save_borrows("test_borrows.csv", borrow_list, 6) == true);

    /* TEST 13 - FREE LIST */
    list_delete(borrow_list, &borrow_deleter);

    /* TEST 14 - RELOAD LIST */
    borrow_list = list_new();
    assert(serializer_load_borrows("test_borrows.csv", borrow_list) == 6);

    /* TEST 15 - PRINT ELEMENTS AFTER RELOAD */
    printf("\nFOREACH #3\n");
    list_foreach(borrow_list, &printer, NULL);

    /* TEST 16 - FREE RELOADED LIST */
    list_delete(borrow_list, &borrow_deleter);
    
    return 0;
}
