# Build a Server Program
cmake_minimum_required(VERSION 3.10)
if(BUILD_CLIENT)
	add_executable(Server main.cpp)
endif()

