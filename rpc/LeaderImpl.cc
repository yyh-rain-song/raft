#icnlude "raft.grpc.pb.h"
#include <iostream>
#include <mutex>
#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/security/server_credentials.h>

using raft::Append_info;
using raft::returnAppend;
using raft::VoteInfo;
using raft::returnVote;

class LeaderImpl
{
public:
    void AppendEntries()
    {

    }
    void VoteRequest()
    {

    }
};