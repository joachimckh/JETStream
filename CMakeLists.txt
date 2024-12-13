cmake_minimum_required(VERSION 3.16)
project(MyProject LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(HOME_DIR $ENV{HOME})

find_package(ROOT REQUIRED COMPONENTS Core RIO Hist)
include_directories(${ROOT_INCLUDE_DIRS})

include_directories(${HOME_DIR}/PYTHIA/pythia8311/include)
include_directories(${HOME_DIR}/alisw/sw/osx_arm64/fastjet/latest/include)
link_directories(${HOME_DIR}/PYTHIA/pythia8311/lib)
link_directories(${HOME_DIR}/alisw/sw/osx_arm64/fastjet/latest/lib)

set(ROOT_ROOTCLING_EXECUTABLE "${HOME_DIR}/alisw/sw/osx_arm64/ROOT/v6-32-06-alice1-local1/bin/rootcling")

add_subdirectory(Core)
add_executable(main Tasks/main.cxx)
target_link_libraries(main PythiaEvent ${ROOT_LIBRARIES} pythia8 fastjet)
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/Core/include)
