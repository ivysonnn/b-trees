cmake_minimum_required(VERSION 3.16)
project(btree)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CPP_STANDARD 11')
set(CMAKE_CPP_STANDARD_REQUIRED ON)
set(CMAKE_CPP_FLAGS "${CMAKE_CXX_FLAGS} -Wall ")

find_package(raylib REQUIRED)

add_library(btr INTERFACE)
target_include_directories(btr INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(${CMAKE_PROJECT_NAME}
	src/main.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
	raylib
    btr
)