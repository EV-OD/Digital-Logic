file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
list(REMOVE_ITEM SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_executable(program WIN32 main.cpp ${SRC_FILES})  # Add WIN32 here


# Create a custom target to copy style.css every time
add_custom_target(copy_style_css ALL
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/style.css
    ${CMAKE_CURRENT_BINARY_DIR}/style.css
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/style.css)

# Ensure the copy target is built before the main executable
add_dependencies(program copy_style_css)

# Copy assets folder from src to build/src folder
add_custom_command(TARGET program POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_CURRENT_BINARY_DIR}/assets)

target_link_libraries(program ${GTKMM_LIBRARIES})
