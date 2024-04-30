cmake_minimum_required(VERSION 3.25)
project(ipoddb)

set(CMAKE_C_STANDARD 99)
add_executable(ipoddb src/ipod_db_builder.c)
target_include_directories(ipoddb PRIVATE inc/)