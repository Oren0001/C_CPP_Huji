cmake_minimum_required(VERSION 3.17)
project(CPP_Projects)

set(CMAKE_CXX_STANDARD 14)

# boost:
find_package (Boost 1.66.0 COMPONENTS unit_test_framework)
include_directories (${Boost_INCLUDE_DIRS})

add_executable(CPP_Projects
        to_test.cpp
#        ex5-oren503/Tests/my_test.cpp
#        ex5-oren503/main.cpp
#        ex5-oren503/Tests/Ex5Tests_1.4.cpp
#        ex5-oren503/Tests/test_success_rate/test_network.cpp
        ex5-oren503/MlpNetwork.h
        ex5-oren503/Matrix.h
        ex5-oren503/Digit.h
        ex5-oren503/Dense.h
        ex5-oren503/Activation.h
        ex5-oren503/Matrix.cpp
        ex5-oren503/Activation.cpp
        ex5-oren503/Dense.cpp
        ex5-oren503/MlpNetwork.cpp)


# boost:
target_link_libraries (CPP_Projects ${Boost_LIBRARIES})