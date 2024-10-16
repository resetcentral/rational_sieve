cmake_minimum_required(VERSION 3.18)
project(rational_sieve)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE "Debug")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  runner
  src/runner.cpp
)
# target_link_libraries(
#   test
#   GTest::gtest_main
#   gmock_main
# )

# include(GoogleTest)
# gtest_discover_tests(test)