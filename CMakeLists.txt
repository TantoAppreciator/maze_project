cmake_minimum_required(VERSION 3.10)
project(maze_generator)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find all source files
file(GLOB SOURCES "src/*.cpp")

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Optional: Add compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()