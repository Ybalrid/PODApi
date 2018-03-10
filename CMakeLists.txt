cmake_minimum_required(VERSION 3.6)

project(PODApi)

file(GLOB PODApi_SRC PODApi/*)
file(GLOB PODApi_H include/*)
file(GLOB PODApi_TEST test/*)

include_directories(./include/)

add_library(PODApi SHARED ${PODApi_SRC} ${PODApi_H})
target_compile_definitions(PODApi PRIVATE DLL_EXPORT)

if(WIN32)
#link dll to the winsock2 library on windows
target_link_libraries(PODApi wsock32 ws2_32)
endif(WIN32)

add_executable(PODApiTest ${PODApi_TEST} ${PODApi_H})
target_link_libraries(PODApiTest PODApi)

add_executable(POD_Simulator ./test_pod_simulator/main.c ./include/packets.h)
target_link_libraries(POD_Simulator wsock32 ws2_32)


