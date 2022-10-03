cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 11)

# set the project name
project(TP2)

# add the lib
add_library(src SHARED vector.cpp)

# add another
# add_library(mysource_2 SHARED sources_2.cpp)

# add the executable
add_executable(main main.cpp)

# do not forget to link
target_link_libraries(main src)

# target_link_libraries(main mysource_2)
