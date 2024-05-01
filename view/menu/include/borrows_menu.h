#ifndef BORROWS_MENU_H
#define BORROWS_MENU_H

#include "list.h"
#include "common.h"

/* Public Functions */

void ui_show_borrows_menu(List borrow_list, List book_list, List user_list, size_t *borrow_last_id, size_t *book_last_id, size_t *user_last_id);

#endif
