add_library(core SHARED)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/src/core/coredict.cxx
           ${CMAKE_BINARY_DIR}/src/core/coredict_rdict.pcm
           ${CMAKE_BINARY_DIR}/src/core/coredict.rootmap
    COMMAND ${ROOT_ROOTCLING_EXECUTABLE} -f ${CMAKE_BINARY_DIR}/src/core/coredict.cxx -c
            ${CMAKE_SOURCE_DIR}/src/core/include/PythiaEvent.hpp
            ${CMAKE_SOURCE_DIR}/src/core/include/coreLinkDef.hpp
    DEPENDS ${CMAKE_SOURCE_DIR}/src/core/include/PythiaEvent.hpp
            ${CMAKE_SOURCE_DIR}/src/core/include/coreLinkDef.hpp
)

target_sources(core PRIVATE ${CMAKE_BINARY_DIR}/src/core/coredict.cxx)
target_include_directories(core PRIVATE ${CMAKE_SOURCE_DIR}/src/core/include)
target_link_libraries(core PRIVATE ${ROOT_LIBRARIES} pythia8 fastjet)

add_subdirectory(src)
