cmake_minimum_required(VERSION 3.0)
project(hm_ipc)

message(STATUS "Enter cmake ${CMAKE_CURRENT_LIST_DIR}")

file(GLOB_RECURSE CODE_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE CODE_SRCS_C ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE CODE_INCS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
list(APPEND CODE_SRCS ${CODE_SRCS_C})
message(STATUS "Source files ${CODE_SRCS}")
message(STATUS "Header files ${CODE_INCS}")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
link_directories(${PROJECT_SOURCE_DIR}/output/lib)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/bin)
set(LIBRARY_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/output/lib)

add_library(${PROJECT_NAME} SHARED ${CODE_SRCS})

add_executable(hm_read ${CODE_INCS} ${CMAKE_CURRENT_SOURCE_DIR}/sample/hm_read.cpp)
target_link_libraries(hm_read ${PROJECT_NAME} pthread)

add_executable(hm_write ${CODE_INCS} ${CMAKE_CURRENT_SOURCE_DIR}/sample/hm_write.cpp)
target_link_libraries(hm_write ${PROJECT_NAME} pthread)

install(DIRECTORY xml/ DESTINATION ${PROJECT_SOURCE_DIR}/output/xml)