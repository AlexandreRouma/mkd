cmake_minimum_required(VERSION 3.13)
project(demo)

file(GLOB_RECURSE SRC "mkd/*.cpp" "demo/*.cpp" "vendor/*.cpp")

add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME} PRIVATE "mkd/")
target_include_directories(${PROJECT_NAME} PRIVATE "demo/")
target_include_directories(${PROJECT_NAME} PRIVATE "vendor/")


target_compile_options(${PROJECT_NAME} PRIVATE /std:c++20)