cmake_minimum_required(VERSION 3.15)
project(libremu8086 CXX)

set(CMAKE_CXX_STANDARD 17)

# generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include(CTest)
add_executable(
        ${PROJECT_NAME}_test
        tests/hello.cpp
)
target_link_libraries(${PROJECT_NAME}_test GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)


add_executable(
        libremu8086
        src/libremu8086.cpp
        src/main.cpp
)

