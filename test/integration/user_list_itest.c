#include <stdio.h>
#include <assert.h>

#include "list.h"
#include "user.h"
#include "serializer.h"

void printer(void *content, void *args) {
    User user = (User) content;
    printf(
        "Name: %s\n",
        user_get_name(user)
    );
}

int main() {
    /* Test Artifacts */
    List user_list = list_new();
    User user = NULL;

    /* List Elements */
    User user01 = user_new(1, "Pedro", "My Street", "+55 22 333 55 00");
    User user02 = user_new(2, "John", "Your Street", "+55 22 113 55 00");
    User user03 = user_new(3, "James", "Our Street", "+55 22 333 33 00");
    User user04 = user_new(4, "Stuart", "Their Street", "+55 22 333 55 11");
    User user05 = user_new(5, "Leon", "No one's Street", "+55 22 333 55 22");

    /* TEST 01 - PUSH BACK INITIAL ELEMENTS */
    assert(list_push_back(user_list, user01) == user01);
    assert(list_push_back(user_list, user02) == user02);
    assert(list_push_back(user_list, user03) == user03);
    assert(list_push_back(user_list, user04) == user04);

    /* TEST 02 - PRINT INITIAL ELEMENTS */
    printf("\nFOREACH #1\n");
    list_foreach(user_list, &printer, NULL);

    /* TEST 04 - GET ELEMENT */
    user = (User) list_get(user_list, 2);
    assert(user == (User) user03);

    /* TEST 05 - REMOVE ELEMENT */
    user = (User) list_remove(user_list, 2);
    assert(user == (User) user03);
    user_delete(user);

    /* TEST 06 - POP BACK */
    user = (User) list_pop_back(user_list);
    assert(user == (User) user04);
    user_delete(user);

    /* TEST 07 - PUSH AFTER POP */
    assert(list_push_back(user_list, user05) == user05);

    /* TEST 08 - PRINT ELEMENTS AFTER VARIOUS OPERATIONS */
    printf("\nFOREACH #2\n");
    list_foreach(user_list, &printer, NULL);

    /* TEST 09 - GET FIRST ELEMENT */
    user = (User) list_get(user_list, 0);
    assert(user == (User) user01);
    assert(list_get(user_list, 0) == list_front(user_list));

    /* TEST 10 - GET LAST ELEMENT */
    user = (User) list_get(user_list, list_size(user_list) - 1);
    assert(user == (User) user05);
    assert(list_get(user_list, list_size(user_list) - 1) == list_back(user_list));

    /* TEST 11 - LIST SIZE */
    assert(list_size(user_list) == 3);

    /* TEST 12 - SAVE LIST */
    assert(serializer_save_users("test_users.csv", user_list, 6) == true);

    /* TEST 13 - FREE LIST */
    list_delete(user_list, &user_deleter);

    /* TEST 14 - RELOAD LIST */
    user_list = list_new();
    assert(serializer_load_users("test_users.csv", user_list) == 6);

    /* TEST 15 - PRINT ELEMENTS AFTER RELOAD */
    printf("\nFOREACH #3\n");
    list_foreach(user_list, &printer, NULL);

    /* TEST 16 - FREE RELOADED LIST */
    list_delete(user_list, &user_deleter);
    
    return 0;
}
