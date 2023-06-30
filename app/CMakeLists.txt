set(APP_SOURCES
    main.cpp
)

add_executable(${EXECUTABLE_NAME} ${APP_SOURCES})
target_link_libraries(${EXECUTABLE_NAME} PUBLIC
    ${LIBRARY_NAME}
    ${CONAN_NLOHMANN_JSON}
    ${CONAN_SPDLOG}
    ${CONAN_CXXOPTS}
    ${CONAN_EIGEN}
    ${CONAN_ZLIB}
    ${TORCH_LIBRARIES}
    date
    ${LIBRARY_FORMAT_NAME}
    ${LIBRARY_TWS981_NAME}
    ${SHAREDMEM_LIB_NAME}
    ${ZMQLIB}
    pthread
    yaml-cpp
)

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${EXECUTABLE_NAME} ENABLE ON AS_ERROR OFF)
endif()
