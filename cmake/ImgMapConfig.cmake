get_filename_component(IMGMAP_CM_DIR "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)
set(IMGMAP_INCLUDE_DIR ${IMGMAP_CM_DIR}/../../include)
set(IMGMAP_LIB_DIR ${IMGMAP_CM_DIR}/../../lib)
include_directories(${IMGMAP_INCLUDE_DIR})
link_directories(${IMGMAP_LIB_DIR})

if(EXISTS "${IMGMAP_LIB_DIR}/libimgmap.so")
  message("-- ImgMap library found")
elseif(EXISTS "${IMGMAP_LIB_DIR}/libimgmap.dylib")
  message("-- ImgMap library found")
else()
  message(SEND_ERROR "Error: ImgMap library not found")
endif()

if(EXISTS "${IMGMAP_INCLUDE_DIR}/imgmap.h")
  message("-- ImgMap headers found")
else()
  message(SEND_ERROR "Error: ImgMap headers not found")
endif()

set(IMGMAP_LIB imgmap)

