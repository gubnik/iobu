find_package(Asio CONFIG REQUIRED)

add_executable(example-00-stl ${CMAKE_CURRENT_SOURCE_DIR}/examples/00-stl.cpp)
target_link_libraries(example-00-stl PRIVATE iobu)
set_target_properties(example-00-stl PROPERTIES CXX_STANDARD 23)
