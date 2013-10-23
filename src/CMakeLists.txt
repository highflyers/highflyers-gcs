add_subdirectory(apconfig)
add_subdirectory(gui)
add_subdirectory(core)

add_executable(GCS main.cpp gui core)

target_link_libraries(GCS gui core)
qt5_use_modules(GCS Widgets)

