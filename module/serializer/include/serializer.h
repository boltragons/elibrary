#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "common.h"
#include "list.h"

/* Public Functions */

bool serializer_save_users(const String file, const List users, size_t last_id);

bool serializer_save_books(const String file, const List books, size_t last_id);

bool serializer_save_borrows(const String file, const List borrows, size_t last_id);

size_t serializer_load_users(const String file, List users);

size_t serializer_load_books(const String file, List books);

size_t serializer_load_borrows(const String file, List borrows);

#endif
