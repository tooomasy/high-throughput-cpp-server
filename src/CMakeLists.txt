# add_subdirectory(base)
# add_subdirectory(core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(CppServer main.cc)

# target_link_libraries(CppServer PRIVATE CppServerBase CppServerCore)
add_custom_command(
    TARGET CppServer POST_BUILD
    COMMAND cp ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/compile_commands.json
)