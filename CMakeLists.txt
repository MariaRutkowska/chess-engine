cmake_minimum_required(VERSION 3.20)
project(chess_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
add_compile_options(-Wall -Wextra -Wpedantic)

# Core engine code as a library, shared by the executable and the tests
add_library(engine_core
  src/board.cpp
  src/uci.cpp
  src/attacks.cpp
  src/movegen.cpp
)
target_include_directories(engine_core PUBLIC src)

add_executable(engine src/main.cpp)
target_link_libraries(engine PRIVATE engine_core)

enable_testing()
add_executable(tests tests/test_board.cpp)
target_link_libraries(tests PRIVATE engine_core)
add_test(NAME board_tests COMMAND tests)
