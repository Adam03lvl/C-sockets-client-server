cmake_minimum_required(VERSION 3.19)

project(ChatApp)

link_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(server src/server.c src/queue.c src/socketutil.c)
add_executable(client src/client.c src/socketutil.c)
