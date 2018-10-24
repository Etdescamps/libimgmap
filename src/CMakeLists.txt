
set(IMGMAP_SRC file_map.c file_parse.c pbm_file.c imgmap.c)
add_library(${IMGMAP_LIB} SHARED ${IMGMAP_SRC})

install(TARGETS ${IMGMAP_LIB} DESTINATION lib)


