add_library(JNetwork SHARED)

target_include_directories(JNetwork PRIVATE ${CMAKE_SOURCE_DIR}/Core/JNetwork/include)
target_link_libraries(JNetwork PRIVATE "${TORCH_LIBRARIES}")

add_subdirectory(src)
