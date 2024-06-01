
file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
list(REMOVE_ITEM SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/main.cc)

add_executable(program main.cpp ${SRC_FILES})

# Copy style.css from src to build folder
add_custom_command(TARGET program POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/style.css
    $<TARGET_FILE_DIR:program>)

# Copy assets folder from src to build folder
add_custom_command(TARGET program POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:program>/assets)

target_link_libraries(program ${GTKMM_LIBRARIES})
