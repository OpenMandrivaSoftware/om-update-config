cmake_minimum_required(VERSION 3.0)
project(om-update-config CXX)

add_subdirectory(src)

install(PROGRAMS om-update-config DESTINATION bin)
