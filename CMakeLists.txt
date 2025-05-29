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
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
  target_link_libraries(${PROJECT_NAME} stdc++fs)
endif()

# Custom run target (this is what allows you to do `make run`)
add_custom_target(run
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
  DEPENDS ${PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# Define distribution directory
set(DIST_DIR "${CMAKE_BINARY_DIR}/dist")

# Custom target for creating distribution package
add_custom_target(dist
  # Create dist directory
  COMMAND ${CMAKE_COMMAND} -E make_directory ${DIST_DIR}
  
  # Copy executable
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${DIST_DIR}/
  
  # Create data directory
  COMMAND ${CMAKE_COMMAND} -E make_directory ${DIST_DIR}/data
  
  # Copy data files
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${DIST_DIR}/data
  
  # Display completion message
  COMMAND ${CMAKE_COMMAND} -E echo "Distribution package created in ${DIST_DIR}/"
  
  DEPENDS ${PROJECT_NAME}
  COMMENT "Creating distribution package in dist/ directory"
)

# Separate target for copying MinGW DLLs (optional)
if(MINGW)
  get_filename_component(MINGW_BIN_PATH ${CMAKE_CXX_COMPILER} DIRECTORY)
  
  add_custom_target(copy-dlls
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
      "${MINGW_BIN_PATH}/libgcc_s_seh-1.dll"
      "${MINGW_BIN_PATH}/libstdc++-6.dll" 
      "${MINGW_BIN_PATH}/libwinpthread-1.dll"
      "${MINGW_BIN_PATH}/libmcfgthread-1.dll"
      ${DIST_DIR}/
    COMMENT "Copying MinGW runtime DLLs"
  )
  
  # Make dist depend on copy-dlls
  add_dependencies(dist copy-dlls)
endif()