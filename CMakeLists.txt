cmake_minimum_required (VERSION 3.0)
project (bzip2)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")

add_compile_options()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(include)

add_executable(
    ${CMAKE_PROJECT_NAME}
    main.cpp 
    src/coder-mtf.cpp
)
target_link_libraries(${CMAKE_PROJECT_NAME} ${GTEST_LIBRARIES} pthread)
