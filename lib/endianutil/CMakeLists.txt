set(LIBRARY_ENDIAN_SOURCES
    endianutil.cpp
)

set(LIBRARY_ENDIAN_HEADERS
    endianutil.h
)

add_library(${LIBRARY_ENDIAN_NAME} STATIC
    ${LIBRARY_ENDIAN_SOURCES}
    ${LIBRARY_ENDIAN_HEADERS})

target_link_libraries(${LIBRARY_ENDIAN_NAME} PUBLIC
)

target_include_directories(${LIBRARY_ENDIAN_NAME} PUBLIC
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include"
)

# we need this due to .inl extension
set_target_properties(${LIBRARY_ENDIAN_NAME} PROPERTIES LINKER_LANGUAGE CXX)
