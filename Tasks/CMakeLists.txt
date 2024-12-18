add_executable(pythia_generator generator.cxx)
target_link_libraries(pythia_generator PRIVATE PythiaEvent ${ROOT_LIBRARIES} pythia8 fastjet "${TORCH_LIBRARIES}")
target_include_directories(pythia_generator PRIVATE ${CMAKE_SOURCE_DIR}/Core/PythiaEvent/include)

add_executable(train_network trainer.cxx)
target_link_libraries(train_network PRIVATE JNetwork PythiaEvent ${ROOT_LIBRARIES} "${TORCH_LIBRARIES}")
target_include_directories(train_network PRIVATE ${CMAKE_SOURCE_DIR}/Core/JNetwork/include ${CMAKE_SOURCE_DIR}/Core/PythiaEvent/include)