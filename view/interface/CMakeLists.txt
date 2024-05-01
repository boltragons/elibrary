# Copyright (c) 2024 eLib
# All rights reserved

set(project_name elib_view_interface)

set(sources_dir ${CMAKE_CURRENT_LIST_DIR}/source)
set(headers_dir ${CMAKE_CURRENT_LIST_DIR}/include)

set(sources
    ${sources_dir}/interface.c
)
set(headers
    ${headers_dir}/interface.h
)

add_library(${project_name} STATIC ${sources} ${headers})

set_target_properties(${project_name} PROPERTIES
    LINKER_LANGUAGE C
    C_STANDARD 11
)

target_include_directories(${project_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(${project_name} LINK_PUBLIC
    elib_utils
)