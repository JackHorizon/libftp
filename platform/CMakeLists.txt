
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(platform STATIC
  posix/posix_mem.c
  posix/posix_socket.c)

target_link_libraries(platform PUBLIC pthread common)
<<<<<<< HEAD
if(WIN32)
  target_link_libraries(platform PUBLIC ws2_32)
else()
  target_link_libraries(platform PUBLIC pthread common)
endif()
=======
>>>>>>> 4626990be1baa6ed1dae6e25ca55e8a4781f5517

message(STATUS "Building for POSIX")

install(TARGETS platform
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)

install(FILES socket.h mem.h
  DESTINATION include
)