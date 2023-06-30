set(LIBRARY_FORMAT_SOURCES
    format.cpp
)

set(LIBRARY_FORMAT_HEADERS
    format.h
)

add_library(${LIBRARY_FORMAT_NAME} STATIC
    ${LIBRARY_FORMAT_SOURCES}
    ${LIBRARY_FORMAT_HEADERS})

target_link_libraries(${LIBRARY_FORMAT_NAME} PUBLIC
    ${CONAN_FMT}
)

target_include_directories(${LIBRARY_FORMAT_NAME} PUBLIC
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include"
)

# we need this due to .inl extension
set_target_properties(${LIBRARY_FORMAT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
