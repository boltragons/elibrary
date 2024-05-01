#include "users_menu.h"

#include <stdio.h>
#include <string.h>

#include "user.h"
#include "interface.h"
#include "serializer.h"
#include "comparators.h"

/* Private Iteration Functions */

static void it_print_user(void *user, void *args) {
    User user_obj = (User) user;
    printf(
        "%-8lu%-50s%-50s%-20s\n", 
        user_get_id(user_obj),
        user_get_name(user_obj),
        user_get_address(user_obj),
        user_get_phone(user_obj)
    );
}

/* Private Menu Functions */

static void ui_show_users_add_menu(List user_list, size_t *user_last_id) {
    /* Function Preambule */
    static StringBuffer buffers[3];

    if(
        READ_TEXT_FIELD_DEF_CLOSE(buffers[0], "user", "name")       &&
        READ_TEXT_FIELD_DEF_CLOSE(buffers[1], "user", "address")    &&
        READ_PHONE_FIELD_DEF_CLOSE(buffers[2], "user", "phone")
    ) {
        User user = user_new(
            *user_last_id,
            buffers[0],
            buffers[1],
            buffers[2]
        );

        if(
            user &&
            list_push_back(user_list, user) &&
            serializer_save_users(USERS_FILE, user_list, ++(*user_last_id))
        ) {
            ui_show_success("User added successfully!");
        }
        else {
            ui_show_error(
                "There was an error adding the user to the system",
                SYSTEM_ERROR_USER_CREATE
            );
        }
    }

    CLEAR_STRING_BUFFER_ARRAY(buffers, 3);
}

static void ui_show_users_list_menu(List user_list, size_t *user_last_id) {
    ui_show_header(NULL);

    if(list_empty(user_list)) {
        ui_show_message("There are no users.");
        return;
    }

    printf(
        "\033[1;40m%-8s%-50s%-50s%-20s\033[0m\n",
        "ID",
        "Name",
        "Address",
        "Phone"
    );
    list_foreach(user_list, &it_print_user, NULL);

    ui_wait_enter();
}

static void ui_show_users_update_menu(List user_list, size_t *user_last_id) {
    /* Function Preambule */
    static StringBuffer buffers[3];
    static size_t id_buffer;
    
    bool some_field_modified = false;

    if(!READ_INTEGER_FIELD_DEF_CLOSE(&id_buffer, "user", "ID")) {
        return;
    }

    User user = (User) list_search(user_list, &it_search_user_by_id, &id_buffer);

    if(!user) {
        ui_show_message("User not found!");
        return;
    }

    if(READ_TEXT_FIELD_DEF_CONTINUE(buffers[0], "user", "name")) {
        user_set_name(user, buffers[0]);
        CLEAR_STRING_BUFFER(buffers[0]);
        some_field_modified = true;
    }

    if(READ_TEXT_FIELD_DEF_CONTINUE(buffers[1], "user", "address")) {
        user_set_address(user, buffers[1]);
        CLEAR_STRING_BUFFER(buffers[1]);
        some_field_modified = true;
    }

    if(READ_PHONE_FIELD_DEF_CONTINUE(buffers[2], "user", "phone")) {
        user_set_phone(user, buffers[2]);
        CLEAR_STRING_BUFFER(buffers[2]);
        some_field_modified = true;
    }

    if(!some_field_modified) {
        return;
    }

    if(
        serializer_save_users(USERS_FILE, user_list, *user_last_id)
    ) {
        ui_show_success("User updated successfully!");
    }
    else {
        ui_show_error(
            "There was an error updating the user",
            SYSTEM_ERROR_USER_UPDATE
        );
    }
}

static void ui_show_users_delete_menu(List user_list, size_t *user_last_id) {
    /* Function Preambule */
    static size_t id_buffer;

    if(!READ_INTEGER_FIELD_DEF_CONTINUE(&id_buffer, "user", "ID")) {
        return;
    }

    if(
        !list_search_remove(user_list, &it_search_user_by_id, &id_buffer, &user_deleter)
    ) {
        ui_show_message("User not found!");
        return;
    }

    if(
        serializer_save_users(USERS_FILE, user_list, *user_last_id)
    ) {
        ui_show_success("User deleted successfully!");
    }
    else {
        ui_show_error(
            "There was an error deleting the user",
            SYSTEM_ERROR_USER_DELETE
        );
    }
}

/* Public Functions */

void ui_show_users_menu(List user_list, size_t *user_last_id) {
    int option = -1;

    do {
        ui_show_header(
            "1) Add user\n"
            "2) List users\n"
            "3) Update user\n"
            "4) Delete user\n"
            "0) Return\n"
            "> "
        );

        scanf("%d", &option);
        common_clean_buffer();

        switch (option) {
            case 0:
                break;
            case 1:
                ui_show_users_add_menu(user_list, user_last_id);
                break;
            case 2:
                ui_show_users_list_menu(user_list, user_last_id);
                break;
            case 3:
                ui_show_users_update_menu(user_list, user_last_id);
                break;
            case 4:
                ui_show_users_delete_menu(user_list, user_last_id);
                break;
            default:
                ui_show_message("Invalid option! Try again.");
        }
    } while (option != 0);
}
