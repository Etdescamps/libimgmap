cmake_minimum_required(VERSION 3.5)

project(libimgmap)

set(VERSION 0.0.1)

enable_language(C)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Select the type of build: Debug Release" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

include(cmake/flags.cmake)
include(cmake/c.cmake)

include_directories(include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)

set(IMGMAP_LIB imgmap)

add_subdirectory(src)
add_subdirectory(tests)

install(FILES include/imgmap.h DESTINATION include/)
install(FILES cmake/ImgMapConfig.cmake DESTINATION share/imgmap)

