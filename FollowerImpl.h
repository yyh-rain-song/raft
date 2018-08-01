#ifndef RAFT_YYH_FOLLOWERIMPL_H
#define RAFT_YYH_FOLLOWERIMPL_H

#include "rpc/raft.grpc.pb.h"
#include "rpc/raft.pb.h"
#include "identity.h"
#include "log.h"
#include <iostream>
#include <algorithm>
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

class FollowerImpl final : public raft::FollowerInteraction::Service
{
public:
    ~FollowerImpl()override{};
    grpc::Status AppendEntries(grpc::ServerContext* context,
            const Append_info* request, returnAppend* response) override;
    grpc::Status VoteRequest(grpc::ServerContext* context,
            const VoteInfo* request, returnVote* response) override;

    void bindAppend(std::function<void(const Append_info*, returnAppend*)> f){
        AppendRPC_ = f;
    }
    void bindVote(std::function<void(const VoteInfo*, returnVote*)> f){
        VoteRPC_ = f;
    }
private:

    std::mutex protect;
    std::function<void(const Append_info*, returnAppend*)> AppendRPC_;
    std::function<void(const VoteInfo*, returnVote*)> VoteRPC_;
};



#endif //RAFT_YYH_FOLLOWERIMPL_H
