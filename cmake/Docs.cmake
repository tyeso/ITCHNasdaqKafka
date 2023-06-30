find_package(Doxygen)

if (DOXYGEN_FOUND)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile   # file we want to configure
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY  # paste it to binary dir
    )
    add_custom_target(docs
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs)
endif()
