# Setup
cmake_minimum_required(VERSION 3.10)
project(OrderProcessFood)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Directory (.h file)
include_directories(include)

# Dynamic collect all cpp file in src folder
file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Build
add_executable(${PROJECT_NAME} ${SOURCES})