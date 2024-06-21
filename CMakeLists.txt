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


# cmake_minimum_required(VERSION 3.5)

# project(example)
# set(CMAKE_CXX_STANDARD 17)

# find_package(PkgConfig REQUIRED)

# pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)
 # look into FindPkgConfig.cmake, 
                                                                # it contains documentation
# Now the variables GTKMM_INCLUDE_DIRS, GTKMM_LIBRARY_DIRS and GTKMM_LIBRARIES 
# contain what you expect




# cmake_minimum_required(VERSION 3.5)
# project(GTKMMTutorial)

# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD 17)

# # set(GTKMM4_INCLUDE_DIRS "C:/msys64/mingw64/include/")
# # set(GTKMM4_LIBRARY_DIRS "C:/msys64/mingw64/lib")
# set(ENV{PKG_CONFIG_PATH} "C:/msys64/mingw64/lib/pkgconfig")

# find_package(PkgConfig REQUIRED)
# pkg_check_modules(GTKMM  REQUIRED gtkmm-4.0)

# include_directories(${GTKMM_INCLUDE_DIRS})
# link_directories(${GTKMM_LIBRARY_DIRS})
# # add_definitions(${GTK4_CFLAGS_OTHER})

# add_executable(TutorialApp src/main.cpp)
# set_target_properties(TutorialApp PROPERTIES WIN32_EXECUTABLE true)  # If you dont want console to run with your .exe
# target_link_libraries(TutorialApp ${GTKMM_LIBRARIES})