#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "user.h"

int main() {
    /* Test Artifacts */
    User user = NULL;

    /* TEST 00 - VERIFY PHONE (USES common.h) */
    assert(common_verify_phone("+55 33 501 35 21") == true);
    assert(common_verify_phone("+55 33 501 35") == false);

    /* TEST 01 - CREATE USER */
    user = user_new(2, "Pedro", "Workers Street", "+55 33 551 85 20");
    assert(user != NULL);

    /* TEST 02 - GETTERS */
    assert(user_get_id(user) == 2);
    assert(strcmp(user_get_name(user), "Pedro") == 0);
    assert(strcmp(user_get_address(user), "Workers Street") == 0);
    assert(strcmp(user_get_phone(user), "+55 33 551 85 20") == 0);

    /* TEST 03 - SETTERS */
    assert(user_set_name(user, "PEDRO") == true);
    assert(user_set_name(user, "") == false);
    assert(user_set_address(user, "MY STREET") == true);
    assert(user_set_address(user, "") == false);
    assert(user_set_phone(user, "+55 33 501 35 21") == true);
    assert(user_set_phone(user, "") == false);
    assert(user_set_phone(user, "+55 33 501 35") == false);

    /* TEST 04 - DELETE */
    user_delete(user);

    return 0;
}