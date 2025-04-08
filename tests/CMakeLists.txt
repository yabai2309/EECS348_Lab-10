# DO NOT MODIFY

include(FetchContent)
FetchContent_Declare(
  googletest
  # branch 1.16.x
  URL https://github.com/google/googletest/archive/e88cb95b92acbdce9b058dd894a68e1281b38495.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests tests.cpp)
target_include_directories(tests PRIVATE ..)
target_link_libraries(tests assignment GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(tests)