option(BUILD_TEST "Build test program" OFF)

if(BUILD_TEST)
<<<<<<< HEAD
  add_executable(test_socket test_socket.c)
  target_include_directories(test_socket PRIVATE ${CMAKE_SOURCE_DIR}/platform)
  target_link_libraries(test_socket PRIVATE platform common)
  message(STATUS "Test program will be built")
endif()
=======
endif()
>>>>>>> 4626990be1baa6ed1dae6e25ca55e8a4781f5517
