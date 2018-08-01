//
// Created by yangyuhuan on 18-7-30.
//

#include "FollowerImpl.h"
grpc::Status FollowerImpl::AppendEntries(grpc::ServerContext* context, const Append_info* request, returnAppend* response)
{
    std::lock_guard<std::mutex> guard(protect);
    AppendRPC_(request, response);
}

grpc::Status FollowerImpl::VoteRequest(grpc::ServerContext* context, const VoteInfo* request, returnVote* response)
{
    std::lock_guard<std::mutex> guard(protect);
    VoteRPC_(request, response);
}
