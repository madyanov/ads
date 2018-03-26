cmake_minimum_required(VERSION 3.10)
project(ads)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCES)
add_executable(ads ${SOURCES})
