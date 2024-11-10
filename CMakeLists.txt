cmake_minimum_required(VERSION 3.27)
project(GOL)

include_directories(.)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

add_executable (GOL main.cpp primitives.cpp)

target_include_directories(GOL PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(GOL PRIVATE ${SDL2_LIBRARIES})

add_compile_options(-g)
