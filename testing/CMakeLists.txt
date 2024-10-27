include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(queue_test Test_Main.cpp Test_Queue.cpp)
target_link_libraries(queue_test GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(queue_test)
