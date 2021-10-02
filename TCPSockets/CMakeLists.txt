cmake_minimum_required(VERSION 3.10)

# Global Configs
option(BUILD_SERVER "Building Server Program .." ON)
option(BUILD_CLIENT "Building Client Program .." ON)


# Global Variables
list(APPEND INCLUDE_DIRS "Server" "Client")

# Project Configs
project(TCPSockets VERSION 1.0)
message("Building Project TCPSockets Version 1.0..")
# Compiler Settings
set(CMAKE_CXX_STANDARD 11)

configure_file(config.h.in config.h)
#foreach(DIR IN LISTS ${INCLUDE_DIRS})
#	message("Adding Subdirectory ${DIR}")
#	add_subdirectory("${DIR}")
#endforeach()
add_subdirectory("Server")
add_subdirectory("Client")

