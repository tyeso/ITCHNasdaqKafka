if (ENABLE_TESTING)
    set(TEST_MAIN
        "unit_tests")
    set(TEST_SOURCES
        "main.cpp"
        # "nasdaq_kafka/nasdaqkafkaTest.cpp"
        "nasdaq_kafka/nasdaqkafkadataTest.cpp"
        )

    add_executable(${TEST_MAIN}
        ${TEST_SOURCES}
        ${TEST_HEADERS})

    target_link_libraries(${TEST_MAIN} PUBLIC
        ${LIBRARY_NAME}
        ${LIBRARY_ITCHKAFKA_NAME}
        ${LIBRARY_ORDERBOOK_NAME}
        ${ZMQLIB}
    )
    target_link_libraries(${TEST_MAIN} PRIVATE
        ${CONAN_CATCH2})

    target_include_directories(${TEST_MAIN} PUBLIC
        "${CMAKE_BINARY_DIR}/configured_files/include"
    )

    if (ENABLE_COVERAGE)
        set(COVERAGE_MAIN "coverage")
        set(COVERAGE_EXCLUDES
            "${PROJECT_SOURCE_DIR}/app/*"
            "${PROJECT_SOURCE_DIR}/cmake/*"
            "${PROJECT_SOURCE_DIR}/docs/*"
            "${PROJECT_SOURCE_DIR}/tests/*"
            "${CONAN_CATCH2_ROOT}/*"
            "${CONAN_CXXOPTS_ROOT}/*"
            "/usr/include/*")
        setup_target_for_coverage_lcov(
            NAME ${COVERAGE_MAIN}
            EXECUTABLE ${TEST_MAIN}
            DEPENDENCIES ${TEST_MAIN}
        )
    endif()
endif()