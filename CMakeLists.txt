cmake_minimum_required(VERSION 3.9)

project(mcmp3)

set(PROJECT_NAME mcmp3)
set(PROJECT_EXAMPLE client-example)

include_directories("./inc")
link_directories("./lib")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SOURCE_FILES "./src/*cc")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_executable(${PROJECT_EXAMPLE} "client-main.cc")

target_link_libraries(${PROJECT_NAME} cid3)
target_link_libraries(${PROJECT_NAME} bass)
target_link_libraries(${PROJECT_NAME} pthread)