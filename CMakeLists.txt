cmake_minimum_required(VERSION 3.5)

project(example)
set(CMAKE_CXX_STANDARD 17)



find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

# Add additional include and link directories here
include_directories(includes)
add_subdirectory(src)


 