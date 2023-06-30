# cmake_minimum_required(VERSION 3.24)
# project(NasdaqKafka)

# # This will file libzmq.so file from /usr/local/lib
# FIND_FILE(ZMQLIB libzmq.so /usr/local/lib)
# IF(NOT ZMQLIB)
#     MESSAGE(SEND_ERROR "Cannot find library libzmq.so.")
# ENDIF(NOT ZMQLIB)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# set(SOURCE_FILES zmq_receiver.cpp)

# add_executable(NasdaqKafka ${SOURCE_FILES})
# # The following line will link with libzmq.so
# TARGET_LINK_LIBRARIES(NasdaqKafka ${ZMQLIB})