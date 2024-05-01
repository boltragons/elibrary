# Copyright (c) 2024 eLib
# All rights reserved

set(project_name elib_view_menu)

set(sources_dir ${CMAKE_CURRENT_LIST_DIR}/source)
set(headers_dir ${CMAKE_CURRENT_LIST_DIR}/include)

set(sources
    ${sources_dir}/books_menu.c
    ${sources_dir}/borrows_menu.c
    ${sources_dir}/reports_menu.c
    ${sources_dir}/users_menu.c
)
set(headers
    ${headers_dir}/books_menu.h
    ${headers_dir}/borrows_menu.h
    ${headers_dir}/reports_menu.h
    ${headers_dir}/users_menu.h
)

add_library(${project_name} STATIC ${sources} ${headers})

set_target_properties(${project_name} PROPERTIES
    LINKER_LANGUAGE C
    C_STANDARD 11
)

target_include_directories(${project_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(${project_name} LINK_PUBLIC
    elib_utils
    elib_module_book
    elib_module_user
    elib_module_borrow
    elib_module_list
    elib_module_serializer
    elib_module_function
    elib_view_interface
)