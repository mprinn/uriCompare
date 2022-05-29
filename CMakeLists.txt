cmake_minimum_required(VERSION 3.10)

project(uri_compare)

add_executable(uri_compare src/main.c src/uri_compare.c include/uri_compare.h)

target_include_directories(uri_compare PUBLIC
                           "include"
                           ${EXTRA_INCLUDES}
                           )