
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(platform STATIC
  posix/posix_mem.c
  posix/posix_socket.c)

target_link_libraries(platform PUBLIC pthread common)

message(STATUS "Building for POSIX")

install(TARGETS platform
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)

install(FILES socket.h mem.h
  DESTINATION include
)