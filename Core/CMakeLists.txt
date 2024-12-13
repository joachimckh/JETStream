add_library(PythiaEvent SHARED)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/CoreDict.cxx
           ${CMAKE_BINARY_DIR}/CoreDict_rdict.pcm
           ${CMAKE_BINARY_DIR}/CoreDict.rootmap
    COMMAND ${ROOT_ROOTCLING_EXECUTABLE} -f ${CMAKE_BINARY_DIR}/CoreDict.cxx -c
            ${CMAKE_SOURCE_DIR}/Core/include/PythiaEvent.h
            ${CMAKE_SOURCE_DIR}/Core/include/CoreLinkDef.h
    DEPENDS ${CMAKE_SOURCE_DIR}/Core/include/PythiaEvent.h
            ${CMAKE_SOURCE_DIR}/Core/include/CoreLinkDef.h
)

target_sources(PythiaEvent PRIVATE ${CMAKE_BINARY_DIR}/CoreDict.cxx)
target_include_directories(PythiaEvent PRIVATE ${CMAKE_SOURCE_DIR}/Core/include)
target_link_libraries(PythiaEvent PRIVATE ${ROOT_LIBRARIES} pythia8 fastjet)

add_subdirectory(src)
