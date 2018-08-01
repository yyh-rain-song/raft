cmake_minimum_required(VERSION 3.12)
project(raft_yyh)



set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}; ${CMAKE_MODULE_PATH}")
include_directories(rpc)

add_executable(raft_yyh main.cpp identity.cpp FollowerImpl.cpp log.cpp rpc/raft.grpc.pb.cc rpc/raft.pb.cc)

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})
find_package(GRPC REQUIRED)
include_directories(${GRPC_INCLUDE_DIR})
target_link_libraries(raft_yyh
        ${Boost_LIBRARIES}
        ${PROTOBUF_LIBRARIES}
        ${GRPC_LIBRARIES}
        )
