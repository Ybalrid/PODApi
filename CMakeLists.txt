cmake_minimum_required(VERSION 3.6)

project(PODApi)

file(GLOB PODApi_SRC PODApi/*)
file(GLOB PODApi_H include/*)
file(GLOB PODApi_TEST test/*)

include_directories(./include/)

add_library(PODApi SHARED ${PODApi_SRC} ${PODApi_H})
target_compile_definitions(PODApi PRIVATE DLL_EXPORT)

add_executable(PODApiTest ${PODApi_TEST} ${PODApi_H})
target_link_libraries(PODApiTest PODApi)


