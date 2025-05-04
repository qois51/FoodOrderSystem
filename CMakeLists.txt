cmake_minimum_required(VERSION 3.10)
project(FoodOrderSystem)

set(CMAKE_CXX_STANDARD 17)

# File sumber
file(GLOB SOURCES src/*.cpp)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Copy data files to build directory
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})

# Include direktori
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Untuk filesystem (jika diperlukan)
target_link_libraries(${PROJECT_NAME} stdc++fs)