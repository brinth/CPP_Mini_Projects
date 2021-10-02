# Build a Server Program
cmake_minimum_required(VERSION 3.10)
if (BUILD_SERVER)
	add_executable(Client main.cpp)
endif()

