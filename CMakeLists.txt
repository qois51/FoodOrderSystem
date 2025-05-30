cmake_minimum_required(VERSION 3.10)
project(FoodOrderSystem)

set(CMAKE_CXX_STANDARD 17)

# File sumber
file(GLOB SOURCES src/*.cpp)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include direktori
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Untuk filesystem (jika diperlukan)
target_link_libraries(${PROJECT_NAME} stdc++fs)

# Custom run target (this is what allows you to do `make run`)
add_custom_target(run
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
  DEPENDS ${PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Define distribution directory
set(DIST_DIR "${CMAKE_BINARY_DIR}/dist")

# Get MinGW DLL path
get_filename_component(MINGW_BIN_PATH ${CMAKE_CXX_COMPILER} DIRECTORY)

# Custom target for creating distribution package
add_custom_target(dist
  # Create dist directory
  COMMAND ${CMAKE_COMMAND} -E make_directory ${DIST_DIR}
  
  # Copy executable
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${DIST_DIR}/
  
  # Copy MinGW runtime DLLs
  COMMAND ${CMAKE_COMMAND} -E copy_if_different 
    "${MINGW_BIN_PATH}/libgcc_s_seh-1.dll"
    "${MINGW_BIN_PATH}/libstdc++-6.dll" 
    "${MINGW_BIN_PATH}/libwinpthread-1.dll"
    "${MINGW_BIN_PATH}/libmcfgthread-1.dll"
    ${DIST_DIR}/
  
  # Create data directory
  COMMAND ${CMAKE_COMMAND} -E make_directory ${DIST_DIR}/data
  
  # Copy data files (if data directory exists in source)
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/data ${DIST_DIR}/data
  
  # Display completion message
  COMMAND ${CMAKE_COMMAND} -E echo "Distribution package created in ${DIST_DIR}/"
  COMMAND ${CMAKE_COMMAND} -E echo "Contents: executable, DLLs, and data files"
  
  DEPENDS ${PROJECT_NAME}
  COMMENT "Creating distribution package with executable, DLLs, and data files"
)

# Optional: Clean dist directory target
add_custom_target(clean-dist
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${DIST_DIR}
  COMMENT "Cleaning distribution directory"
)
