add_library(common STATIC
  com_log.c
  com_string.c
)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
