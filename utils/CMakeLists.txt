# Copyright (c) 2024 eLib
# All rights reserved

set(project_name elib_utils)

set(sources_dir ${CMAKE_CURRENT_LIST_DIR}/source)
set(headers_dir ${CMAKE_CURRENT_LIST_DIR}/include)

set(sources
    ${sources_dir}/common.c
    ${sources_dir}/strman.c
)
set(headers
    ${headers_dir}/common.h
    ${headers_dir}/logger.h
    ${headers_dir}/strman.h
)

add_library(${project_name} STATIC ${sources} ${headers})

set_target_properties(${project_name} PROPERTIES
    LINKER_LANGUAGE C
    C_STANDARD 11
)

target_include_directories(${project_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
