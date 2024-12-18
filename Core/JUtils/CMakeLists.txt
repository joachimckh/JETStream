add_library(JUtils SHARED)

target_include_directories(JUtils PRIVATE ${CMAKE_SOURCE_DIR}/Core/JUtils/include)
target_link_libraries(JUtils PRIVATE)

add_subdirectory(src)
