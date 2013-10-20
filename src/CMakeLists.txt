add_subdirectory(apconfig)

add_executable(GCS main.cpp)

qt5_use_modules(GCS Widgets)

