find_package(Catch2 3 REQUIRED)

# add_executable(
# CppServerTestCases

# # MutexQueue_Test.cc
# # ThreadPool_Test.cc
# )
# target_link_libraries(
# CppServerTestCases
# Catch2::Catch2WithMain
# )
add_test(NAME CppServerTestCases COMMAND CppServerTestCases)