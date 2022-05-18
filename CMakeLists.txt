cmake_minimum_required(VERSION 3.14)

project(roverlang)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src/lexer)
add_subdirectory(src/parser)

add_executable(rover src/main.cpp)
target_link_libraries(rover PRIVATE lexer parser)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  lexer_test
  test/lexer_test.cpp
)
target_link_libraries(
  lexer_test
  gtest_main
  lexer
)

include(GoogleTest)
gtest_discover_tests(lexer_test)
