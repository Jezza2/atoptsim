find_package(OpenMP)

add_executable(rainbow_simulator
    main.cpp
    image.cpp
    sphere.cpp
    camera.cpp
    hittable.cpp
)

target_compile_options(rainbow_simulator PRIVATE -O3 -Wall)
target_compile_features(rainbow_simulator PRIVATE cxx_std_14)

if (OpenMP_FOUND)
    target_link_libraries(rainbow_simulator PRIVATE OpenMP::OpenMP_CXX)
else()
    target_compile_definitions(rainbow_simulator PRIVATE NO_OPENMP)
    message(STATUS "OpenMP not found. Recommend installing for improved performance.")
endif()
