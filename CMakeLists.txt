cmake_minimum_required(VERSION 3.18)
project(JETStream LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HOME_DIR $ENV{HOME})

set(CMAKE_PREFIX_PATH "${HOME_DIR}/PyTorchCXX/libtorch" CACHE PATH "Path to libtorch")
find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS} -O3")

find_package(ROOT REQUIRED COMPONENTS Core RIO Hist)
include_directories(${ROOT_INCLUDE_DIRS})

include_directories(${HOME_DIR}/PYTHIA/pythia8311/include)
include_directories(${HOME_DIR}/alisw/sw/osx_arm64/fastjet/latest/include)
link_directories(${HOME_DIR}/PYTHIA/pythia8311/lib)
link_directories(${HOME_DIR}/alisw/sw/osx_arm64/fastjet/latest/lib)


set(ROOT_ROOTCLING_EXECUTABLE "${HOME_DIR}/alisw/sw/osx_arm64/ROOT/v6-32-06-alice1-local1/bin/rootcling")

add_subdirectory(Core)
add_subdirectory(Tasks)
