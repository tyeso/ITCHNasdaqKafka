set(LIBRARY_SOURCES
    misc.cpp
)

set(LIBRARY_HEADERS
    misc.h
)

add_library(${LIBRARY_NAME} STATIC
    ${LIBRARY_SOURCES}
    ${LIBRARY_HEADERS})

target_include_directories(${LIBRARY_NAME} PUBLIC
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include"
)
