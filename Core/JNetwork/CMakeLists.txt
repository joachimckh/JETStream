add_library(JNetwork SHARED)

target_include_directories(JNetwork PRIVATE 
                           ${CMAKE_SOURCE_DIR}/Core/JNetwork/include 
                           ${CMAKE_SOURCE_DIR}/Core/PythiaEvent/include
                           ${CMAKE_SOURCE_DIR}/Core/JUtils/include
                           )
target_link_libraries(JNetwork PRIVATE "${TORCH_LIBRARIES}" ${ROOT_LIBRARIES} PythiaEvent JUtils)

add_subdirectory(src)
