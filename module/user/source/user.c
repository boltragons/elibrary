#include "user.h"

#include <regex.h>
#include <string.h>

#include "strman.h"

/* Data Types Implementation */

typedef struct user_t {
    size_t id;
    String name;
    String address;
    Phone phone;
} user_t;

/* Public Functions */

User user_new(size_t id, const String name, const String address, const String phone) {
    /* Create receptacle */
    user_t user = {
        .id = id,
        .name = NULL,
        .address = NULL
        /* .phone is fixed size, don't need realloc */
    };

    if(
        !user_set_name(&user, name) ||
        !user_set_address(&user, address) ||
        !user_set_phone(&user, phone)
    ) {
        return NULL;
    }

    /* If everything is OK, copy to object */
    user_t *user_obj = new(user_t);
    memcpy(user_obj, &user, sizeof(user_t));

    return user_obj;
}

void user_delete(User user) {
    free(user->name);
    free(user->address);
    free(user);
}

size_t user_get_id(const User user) {
    return user->id;
}

const String user_get_name(const User user) {
    if(!user) {
        return NULL;
    }
    return user->name;
}

const String user_get_address(const User user) {
    if(!user) {
        return NULL;
    }
    return user->address;
}

const String user_get_phone(const User user) {
    if(!user) {
        return NULL;
    }
    return user->phone;
}

bool user_set_name(User user, const String name) {
    if(!user || !name || strlen(name) == 0) {
        return false;
    }

    strman_copy(&user->name, name);
    return true;
}

bool user_set_address(User user, const String address) {
    if(!user || !address || strlen(address) == 0) {
        return false;
    }

    strman_copy(&user->address, address);
    return true;
}

bool user_set_phone(User user, const String phone) {
    if(!user || !phone || !common_verify_phone(phone)) {
        return false;
    }

    strcpy(user->phone, phone);
    return true;
}

/* Public Wrappers */

void user_deleter(void *user) {
    user_delete((User) user);
}
