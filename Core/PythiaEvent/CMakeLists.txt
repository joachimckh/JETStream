add_library(PythiaEvent SHARED)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/Core/PythiaEvent/CoreDict.cxx
           ${CMAKE_BINARY_DIR}/Core/PythiaEvent/CoreDict_rdict.pcm
           ${CMAKE_BINARY_DIR}/Core/PythiaEvent/CoreDict.rootmap
    COMMAND ${ROOT_ROOTCLING_EXECUTABLE} -f ${CMAKE_BINARY_DIR}/Core/PythiaEvent/CoreDict.cxx -c
            ${CMAKE_SOURCE_DIR}/Core/PythiaEvent/include/PythiaEvent.hpp
            ${CMAKE_SOURCE_DIR}/Core/PythiaEvent/include/CoreLinkDef.hpp
    DEPENDS ${CMAKE_SOURCE_DIR}/Core/PythiaEvent/include/PythiaEvent.hpp
            ${CMAKE_SOURCE_DIR}/Core/PythiaEvent/include/CoreLinkDef.hpp
)

target_sources(PythiaEvent PRIVATE ${CMAKE_BINARY_DIR}/Core/PythiaEvent/CoreDict.cxx)
target_include_directories(PythiaEvent PRIVATE ${CMAKE_SOURCE_DIR}/Core/PythiaEvent/include)
target_link_libraries(PythiaEvent PRIVATE ${ROOT_LIBRARIES} pythia8 fastjet)

add_subdirectory(src)
