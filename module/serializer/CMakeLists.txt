# Copyright (c) 2024 eLib
# All rights reserved

set(project_name elib_module_serializer)

set(sources_dir ${CMAKE_CURRENT_LIST_DIR}/source)
set(headers_dir ${CMAKE_CURRENT_LIST_DIR}/include)

set(sources
    ${sources_dir}/serializer.c
)
set(headers
    ${headers_dir}/serializer.h
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
)