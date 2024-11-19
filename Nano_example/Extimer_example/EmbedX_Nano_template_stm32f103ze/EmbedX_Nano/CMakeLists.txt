
set(LIB_NAME "EmbedX_Nano")
set(SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/")
file(GLOB_RECURSE C_FILES "${SRC_DIR}*.c")

message("--------------------------------")
message("${C_FILES}")
message("--------------------------------")

add_library(${LIB_NAME} STATIC "")



target_sources(${LIB_NAME} PUBLIC "${C_FILES}")

target_include_directories(${LIB_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/component/ExTimer")


