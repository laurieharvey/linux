cmake_minimum_required(VERSION 3.0.0)
project(linux VERSION 0.1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_BUILD_TYPE Debug)

include_directories(include)

add_executable(tee src/tee.c)
add_executable(seek src/lib.c src/seek.c)
add_executable(cp src/lib.c src/cp.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
