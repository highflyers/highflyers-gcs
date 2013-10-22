add_subdirectory(apconfig)
add_subdirectory(gui)

add_executable(GCS main.cpp gui)

target_link_libraries(GCS gui)
qt5_use_modules(GCS Widgets)

