cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

project (Tree)

if (NOT TARGET Tree)

add_library (${PROJECT_NAME} INTERFACE)

target_include_directories (${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/headers)
add_subdirectory (libs)

endif ()
