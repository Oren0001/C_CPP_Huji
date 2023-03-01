cmake_minimum_required(VERSION 3.17)
project(C_Projects)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra -Wvla -Werror -lm")
set(CMAKE_CXX_STANDARD 17)

# boost:
find_package (Boost 1.66.0 COMPONENTS unit_test_framework)
include_directories (${Boost_INCLUDE_DIRS})

#add_executable(C_Projects to_test.c)
add_executable(C_Projects ex4-oren503/vector.c
#        ex4-oren503/main.c
        ex4-oren503/Tests/my_main.c
        ex4-oren503/hash_funcs.h
        ex4-oren503/hashmap.c
        ex4-oren503/hashmap.h
        ex4-oren503/pair.c
        ex4-oren503/pair.h
        ex4-oren503/pair_char_int.h
        ex4-oren503/test_pairs.h
        ex4-oren503/test_suite.c
        ex4-oren503/test_suite.h
        ex4-oren503/vector.h)


#add_executable(C_Projects ex4-oren503/Tests/my_tests.cpp)

# boost:
target_link_libraries (C_Projects ${Boost_LIBRARIES})
