cmake_minimum_required(VERSION 3.0.0)
project(linux VERSION 0.1.0)

add_executable(tee src/tee.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
