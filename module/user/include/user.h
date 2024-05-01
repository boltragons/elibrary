#ifndef USER_H
#define USER_H

#include "common.h"

/* Data Types */

/* Forward Declaration */
struct user_t;

/* Object Definition */
typedef struct user_t* User;

/* Public Functions */

User user_new(size_t id, const String name, const String address, const String phone);

void user_delete(User user);

size_t user_get_id(const User user);

const String user_get_name(const User user);

const String user_get_address(const User user);

const String user_get_phone(const User user);

bool user_set_name(User user, const String name);

bool user_set_address(User user, const String address);

bool user_set_phone(User user, const String phone);

/* Public Wrappers */

void user_deleter(void *user);

#endif
