cmake_minimum_required(VERSION 3.13)

project(uri_compare)

set (libFiles src/uri_compare.c include/uri_compare.h)

add_executable(uri_compare ${libFiles} src/main.c  )

target_include_directories(uri_compare PUBLIC
                           "include"
                           ${EXTRA_INCLUDES}
                           )

add_library(uriCompareDLL SHARED ${libFiles})
add_library(uriCompareLIB ${libFiles})

target_include_directories(uriCompareDLL PUBLIC
                           "include"
                           ${EXTRA_INCLUDES}
                           )

target_include_directories(uriCompareLIB PUBLIC
                           "include"
                           ${EXTRA_INCLUDES}
                           )

include(GenerateExportHeader)
generate_export_header(uriCompareDLL)

set_target_properties(uriCompareLIB PROPERTIES
                             COMPILE_FLAGS -DLIBSHARED_AND_STATIC_STATIC_DEFINE)