cmake_minimum_required(VERSION 3.5)

project(libimgmap)

set(VERSION 0.0.1)

enable_language(C Fortran CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Select the type of build: Debug Release" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

include(cmake/flags.cmake)
include(cmake/fortran.cmake)
include(cmake/c.cmake)
include(cmake/cpp.cmake)

#find_package(LAPACK REQUIRED)
#find_package(HDF5 COMPONENTS C Fortran REQUIRED)
#find_package(Boost 1.54 REQUIRED)

include_directories(include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)
#include_directories(${HDF5_INCLUDE_DIRS})
#set(IMGMAP_INCLUDES ${CMAKE_BINARY_DIR}/imgmap)
#include_directories(${IMGMAP_INCLUDES})


set(IMGMAP_LIB imgmap)

add_subdirectory(src)
add_subdirectory(tests)
#add_subdirectory(utils)
#add_subdirectory(examples)

install(FILES include/imgmap.h DESTINATION include/)
install(FILES cmake/ImgMapConfig.cmake DESTINATION share/imgmap)

