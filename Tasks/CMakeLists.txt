add_executable(pythia_generator generator.cxx)
target_link_libraries(pythia_generator PRIVATE PythiaEvent ${ROOT_LIBRARIES} pythia8 fastjet "${TORCH_LIBRARIES}")
target_include_directories(pythia_generator PRIVATE ${CMAKE_SOURCE_DIR}/Core/include)

# add_executable(main_analysis main_analysis.cxx)
# target_link_libraries(main_analysis PRIVATE PythiaEvent ${ROOT_LIBRARIES} pythia8 fastjet "${TORCH_LIBRARIES}")
# target_include_directories(main_analysis PRIVATE ${CMAKE_SOURCE_DIR}/Core/include)