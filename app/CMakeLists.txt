# Copyright (c) 2024 eLib
# All rights reserved

set(project_name elib_app)

set(sources_dir ${CMAKE_CURRENT_LIST_DIR}/source)

set(sources
    ${sources_dir}/main.c
)

set(elib_executable eLib)

add_executable(${elib_executable} ${sources})

target_link_libraries(${elib_executable} LINK_PUBLIC
    elib_utils
    elib_module_book
    elib_module_user
    elib_module_borrow
    elib_module_list
    elib_module_serializer
    elib_module_function
    elib_view_interface
    elib_view_menu
)