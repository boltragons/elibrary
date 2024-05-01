#include "serializer.h"

#include <stdio.h>

#include "user.h"
#include "book.h"
#include "borrow.h"

/* Private Data Types */

typedef IterationFunction SaverFunction;

typedef void (*LoaderFunction)(const String, List);

/* General Private Functions */

static bool serializer_save(const String file, List list, SaverFunction saver, size_t last_id) {
    if(!file || !list || !saver) {
        return false;
    }

    FILE *database_file = fopen(file, "w+");

    if(!database_file) {
        return false;
    }

    list_foreach(list, saver, database_file);

    fprintf(database_file, "%ld", last_id);

    fclose(database_file);

    return true;
}

static size_t serializer_load(const String file, List list, LoaderFunction loader) {
    if(!file || !list || !loader) {
        return 0;
    }

    static ObjectString object_line;

    size_t last_id = 1;

    FILE *database_file = fopen(file, "r");

    if(!database_file) {
        return last_id;
    }

    while(fgets(object_line, sizeof(ObjectString), database_file)) {
        if(common_parse_number(object_line, &last_id)) {
            break;
        }
        loader(object_line, list);
    }

    fclose(database_file);

    return last_id;
}

/* User Private Functions */

static void serializer_user_saver(void *user, void *args) {
    User user_obj = (User) user; 
    FILE *file = (FILE *) args;
    fprintf(
        file,
        "%lu,%s,%s,%s\n",
        user_get_id(user_obj),
        user_get_name(user_obj),
        user_get_address(user_obj),
        user_get_phone(user_obj)
    );
}

static void serializer_user_loader(const String line, List users) {
    static StringBuffer object_fields[4];
    sscanf(
        line,
        "%[^,],%[^,],%[^,],%[^\n]",
        object_fields[0],
        object_fields[1],
        object_fields[2],
        object_fields[3]
    );
    User user = user_new(
        atoi(object_fields[0]),
        object_fields[1],
        object_fields[2],
        object_fields[3]
    );
    list_push_back(users, user);
}

/* Book Private Functions */

static void serializer_book_saver(void *book, void *args) {
    Book book_obj = (Book) book; 
    FILE *file = (FILE *) args;
    fprintf(
        file,
        "%lu,%s,%s,%s,%lu,%lu\n",
        book_get_id(book_obj),
        book_get_title(book_obj),
        book_get_author(book_obj),
        book_get_genre(book_obj),
        book_get_quantity(book_obj),
        book_get_available(book_obj)
    );
}

static void serializer_book_loader(const String line, List books) {
    static StringBuffer object_fields[6];
    sscanf(
        line,
        "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",
        object_fields[0],
        object_fields[1],
        object_fields[2],
        object_fields[3],
        object_fields[4],
        object_fields[5]
    );
    Book book = book_new(
        atoi(object_fields[0]),
        object_fields[1],
        object_fields[2],
        object_fields[3],
        atoi(object_fields[4])
    );
    book_set_available(book, atoi(object_fields[5]));
    list_push_back(books, book);
}

/* Borrow Private Functions */

static void serializer_borrow_saver(void *borrow, void *args) {
    Borrow borrow_obj = (Borrow) borrow; 
    FILE *file = (FILE *) args;
    fprintf(
        file,
        "%lu,%lu,%lu,%s,%s,%d\n",
        borrow_get_id(borrow_obj),
        borrow_get_user_id(borrow_obj),
        borrow_get_book_id(borrow_obj),
        borrow_get_date(borrow_obj),
        borrow_get_return_date(borrow_obj),
        borrow_has_returned(borrow_obj)
    );
}

static void serializer_borrow_loader(const String line, List borrows) {
    static StringBuffer object_fields[6];
    sscanf(
        line,
        "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",
        object_fields[0],
        object_fields[1],
        object_fields[2],
        object_fields[3],
        object_fields[4],
        object_fields[5]
    );
    Borrow borrow = borrow_new(
        atoi(object_fields[0]),
        atoi(object_fields[1]),
        atoi(object_fields[2]),
        object_fields[3],
        object_fields[4]
    );
    if(atoi(object_fields[5])) {
        borrow_return(borrow);
    }
    list_push_back(borrows, borrow);
}

/* Public Functions */

bool serializer_save_users(const String file, const List users, size_t last_id) {
    return serializer_save(file, users, &serializer_user_saver, last_id);
}

bool serializer_save_books(const String file, const List books, size_t last_id) {
    return serializer_save(file, books, &serializer_book_saver, last_id);
}

bool serializer_save_borrows(const String file, const List borrows, size_t last_id) {
    return serializer_save(file, borrows, &serializer_borrow_saver, last_id);
}

size_t serializer_load_users(const String file, List users) {
    return serializer_load(file, users, &serializer_user_loader);
}

size_t serializer_load_books(const String file, List books) {
    return serializer_load(file, books, &serializer_book_loader);
}

size_t serializer_load_borrows(const String file, List borrows) {
    return serializer_load(file, borrows, &serializer_borrow_loader);
}
