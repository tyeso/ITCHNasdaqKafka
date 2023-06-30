if(APPLE)
    cmake_minimum_required(VERSION 3.24)
    set(CMAKE_HOST_SYSTEM_PROCESSOR=arm64)
    set(MAKE_SYSTEM_PROCESSOR=arm64)
   # set(CMAKE_OSX_ARCHITECTURES "arm64-apple-macos")
else()
    cmake_minimum_required(VERSION 3.18)
endif()

# using clang
if(APPLE)
    set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm@12/bin/clang")
    set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm@12/bin/clang++")
else()
    set(CMAKE_C_COMPILER "/usr/bin/clang-12")
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++-12")
endif()
project(axetrader VERSION 1.0.0 LANGUAGES C CXX)

# checking build type and setting debug/release macro
if (NOT CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "BUILD_TYPE not set!")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions("DEBUG_MODE")
elseif( CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions("RELEASE_MODE")
else()
    message(FATAL_ERROR "Unknown Mode")
endif()

############################################
# set some global variables for our system #
############################################
if (NOT TZ)
    message(WARNING "TZ not set! Setting it to America/New_York")
    set(TZ "America/New_York")
endif()


# C CXX Standards
set(CMAKE_CXX_STANDARD              17)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS   ON)

# libtorch
list(APPEND CMAKE_PREFIX_PATH "/opt/libtorch")
find_package(PythonInterp REQUIRED)
find_package(CUDAToolkit)
find_package(Torch REQUIRED CONFIG)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

# zstd
#find_package(zstd REQUIRED)

# libzmq
FIND_FILE(ZMQLIB libzmq.so /usr/local/lib)
IF(NOT ZMQLIB)
    MESSAGE(SEND_ERROR "Cannot find library libzmq.so.")
ENDIF(NOT ZMQLIB)

# options
option(USE_CONAN            "Whether to use conan or vcpkg"             ON)
option(ENABLE_WARNINGS      "Enable warnings to a target"               ON)
option(ENABLE_TESTING       "Whether to enable the unit testing build"  OFF)
option(ENABLE_COVERAGE      "Enable a Code coverage build"              OFF)
option(ENABLE_CLANG_TIDY    "Enable to add clang tidy"                  OFF)
option(ENABLE_CPPCHECK      "Enable to add cppcheck"                    ON)
# Feature of compiler for Linker optimization while linking different libraries
# (e.g. if a code path is never visited in the library, LTO removes those codes)
option(ENABLE_LTO           "Enable to add Link Time Optimization"      ON)
# Caches your libraries in temporary directory to make rebuilding faster
option(ENABLE_CCACHE        "Eanble to add Ccache"                      ON)
# option to run memory/address sanitizer
option(ENABLE_ASAN          "Enable address sanitizer"                  OFF)

if(ENABLE_ASAN)
    message("Using address sanitizer (checks oob accesses, use after free, \n\
    use after return, use after scope, double free, invalid free, memory leaks")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fsanitize=address -fno-omit-frame-pointer")
endif()


# project library names
# Libraries
set(LIBRARY_NAME Library)
set(SHAREDMEM_LIB_NAME sharedmem)
set(LIBFIN_NAME fin)
set(LIBSIGNAL_NAME signalbase)
set(LIBTRADER_NAME trader)
set(LIBRARY_TWS981_NAME tws981)
set(LIBRARY_TWS981_SAMPLES_NAME tws981_samples)
set(LIBRARY_SIGNAL_NAME signal)
set(LIBRARY_FORMAT_NAME format_header_only)
set(LIBRARY_ITCHKAFKA_NAME itchkafka)
set(LIBRARY_ENDIAN_NAME endianutil)
set(LIBRARY_ORDERBOOK_NAME orderbook)

# data client names
set(MAYSTREET_DATACLIENT_EXECUTABLE_NAME maystreetclient)

# executable names
set(EXECUTABLE_NAME main)
set(IBKR_DATACLIENT_EXECUTABLE_NAME ibkrclient)
set(TRADER_EXECUTABLE_NAME axetrader)
set(BOLLINGERBAND_EXECUTABLE_NAME bollingerband)
set(DONCHIANCHANNEL_EXECUTABLE_NAME donchianchannel)
set(DQN_EXECUTABLE_NAME dqn)
set(MACD_EXECUTABLE_NAME macd)
set(SUPERVISED_CONV_EXECUTABLE_NAME supervised_conv)
set(WSIG_EXECUTABLE_NAME wsig)

# cmake modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
include(Warnings)
include(Docs)
include(AddGitSubmodule)
include(FetchContent)

if(ENABLE_COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags()
    # gcov throws an error when using clang so switch to gcc if
    # we are doing code coverage test
    set(CMAKE_C_COMPILER "/usr/bin/gcc")
    set(CMAKE_CXX_COMPILER "/usr/bin/g++")
endif()

# external libraries
FetchContent_Declare(
    date
    GIT_REPOSITORY https://github.com/HowardHinnant/date
    GIT_TAG v3.0.1
)
FetchContent_MakeAvailable(
    date
)

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp
    GIT_TAG yaml-cpp-0.7.0
)
FetchContent_MakeAvailable(
    yaml-cpp
)

# moodycamel single producer, single consumer queue
FetchContent_Declare(
  readerwriterqueue
  GIT_REPOSITORY    https://github.com/cameron314/readerwriterqueue
  GIT_TAG           master
)

FetchContent_MakeAvailable(readerwriterqueue)

# moody camel concurrent queue
FetchContent_Declare(
  concurrentqueue
  GIT_REPOSITORY    https://github.com/cameron314/concurrentqueue
  GIT_TAG           master
)

FetchContent_MakeAvailable(concurrentqueue)


FetchContent_Declare(ZStrGitRepo
  GIT_REPOSITORY    "https://github.com/mateidavid/zstr" # can also be a local filesystem path!
  GIT_TAG           "master"
)
FetchContent_MakeAvailable(ZStrGitRepo) # defines INTERFACE target 'zstr::zstr'


# conan
if (USE_CONAN)
    message("Using Conan")
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endif()

# run subdirectory CMakeLists.txt
add_subdirectory(app)
add_subdirectory(lib)
add_subdirectory(configured)
add_subdirectory(template)
add_subdirectory(tests)
add_subdirectory(ServerProgram)