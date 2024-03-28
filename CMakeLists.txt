cmake_minimum_required(VERSION 3.19)

project(ChatApp)

link_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(server src/server.c src/queue.c)
add_executable(client src/client.c)

target_link_libraries(server socketutil)
target_link_libraries(client socketutil)
