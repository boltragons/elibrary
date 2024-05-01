#include <stdio.h>
#include <assert.h>

#include "list.h"

bool comparer(void *content, void *args) {
    int *list_value = (int *) content;
    int *compare_value = (int *) args;

    return *list_value == *compare_value;
}

void printer(void *content, void *args) {
    printf("VALUE: %d\n", *((int *) content));
}

void deleter(void *content) {
    free(content);
}

int main() {
    /* Test Artifacts */
    void *object = NULL;
    List list = NULL;

    /* List Elements */
    int *t0 = new(int);
    *t0 = 0;
    int *t1 = new(int);
    *t1 = 1;
    int *t2 = new(int);
    *t2 = 2;
    int *t3 = new(int);
    *t3 = 3;
    int *t4 = new(int);
    *t4 = 4;
    int *t5 = new(int);
    *t5 = 5;

    /* TEST 01 - CREATE LIST */
    list = list_new();
    assert(list != NULL);

    /* TEST 02 - PUSH BACK INITIAL ELEMENTS */
    assert(list_push_back(list, t0) == t0);
    assert(list_push_back(list, t1) == t1);
    assert(list_push_back(list, t2) == t2);
    assert(list_push_back(list, t3) == t3);
    assert(list_push_back(list, t4) == t4);

    /* TEST 03 - PRINT INITIAL ELEMENTS */
    printf("\nFOREACH #1\n");
    list_foreach(list, &printer, NULL);

    /* TEST 04 - GET ELEMENT */
    object = list_get(list, 2);
    assert(object != NULL);
    assert(*((int *) object) == 2);

    /* TEST 05 - REMOVE ELEMENT */
    object = list_remove(list, 2);
    assert(object != NULL);
    assert(*((int *) object) == 2);
    free(object);

    /* TEST 06 - POP BACK */
    object = list_pop_back(list);
    assert(object != NULL);
    assert(*((int *) object) == 4);
    free(object);

    /* TEST 07 - PUSH AFTER POP */
    assert(list_push_back(list, t5) == t5);

    /* TEST 08 - PRINT ELEMENTS AFTER VARIOUS OPERATIONS */
    printf("\nFOREACH #2\n");
    list_foreach(list, &printer, NULL);

    /* TEST 09 - GET FIRST ELEMENT */
    object = list_get(list, 0);
    assert(object != NULL);
    assert(*((int *) object) == 0);
    assert(list_get(list, 0) == list_front(list));

    /* TEST 10 - GET LAST ELEMENT */
    object = list_get(list, list_size(list) - 1);
    assert(object != NULL);
    assert(*((int *) object) == 5);
    assert(list_get(list, list_size(list) - 1) == list_back(list));

    /* TEST 11 - LIST SIZE */
    assert(list_size(list) == 4);

    /* TEST 12 - LIST SEARCH */
    assert(list_search(list, &comparer, t5) == t5);
    assert(list_search(list, &comparer, t4) == NULL);

    /* TEST 13 - LIST SEARCH AND REMOVE */
    assert(list_search_remove(list, &comparer, t5, &free) == true);
    assert(list_search_remove(list, &comparer, t4, &free) == false);

    /* TEST 14 - FREE LIST */
    list_delete(list, &deleter);
    
    return 0;
}
