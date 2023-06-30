set(LIBRARY_ORDERBOOK_SOURCES
    orderbook.cpp
)

set(LIBRARY_ORDERBOOK_HEADERS
    orderbook.h
)

add_library(${LIBRARY_ORDERBOOK_NAME} STATIC
    ${LIBRARY_ORDERBOOK_SOURCES}
    ${LIBRARY_ORDERBOOK_HEADERS})

target_link_libraries(${LIBRARY_ORDERBOOK_NAME} PUBLIC
    ${LIBRARY_ITCHKAFKA_NAME}
)

target_include_directories(${LIBRARY_ORDERBOOK_NAME} PUBLIC
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include"
)
