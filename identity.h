#ifndef RAFT_YYH_IDENTITY_H
#define RAFT_YYH_IDENTITY_H

#include <cstdio>
#include <ctime>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <condition_variable>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/timer.hpp>
#include <random>
#include "log.h"
#include "rpc/raft.pb.h"
#include "rpc/raft.grpc.pb.h"

namespace yyh_raft {

    using raft::Append_info;
    using raft::returnAppend;
    using raft::VoteInfo;
    using raft::returnVote;

    /*
    class identity_base {
    public:
        virtual void identity_transfer() = 0;

        virtual void detectTime() = 0;

        virtual void detectHeart() = 0;

        identity_base();

        enum idType {
            Learder, Follower, Candidate
        };
    protected:
        idType Type;
        std::int64_t elecTimeout;
        std::int64_t elecTurn;
        struct {
            bool timeout;
            std::mutex lock_;
            std::condition_variable lock_cond;
        } dect_timeout;
        int64_t Time;
    };

    class follower : public identity_base {
    public:
        void detectTime();

        void detectHeart();

        void identity_transfer();

        follower();
    };

    class candidate: public identity_base{
    public:
        void identity_transfer();

        void detectHeart();

        void detectTime();

        candidate();
    };
    */
    inline std::int64_t intRand(std::int64_t lb, std::int64_t ub) {
        static thread_local std::random_device rd;
        static thread_local std::mt19937 generator(rd());
        std::uniform_int_distribution<std::int64_t> distribution(lb, ub);
        return distribution(generator);
    }

    class identity_base
    {
    public:
        enum Type{Follower, Candidate, Leader};
        int64_t Term(){return currentTerm;}
        int64_t currentTerm;
        int32_t votedFor;
        std::vector<log> stored_logs;
        int64_t commitIndex;
        int64_t lastApplied;
        boost::timer Time;
        int32_t elecTimeout;
        Type type;
        std::vector<std::thread> threads;
        bool votedThisTurn;
        std::string ID;

        void debug_out();

        identity_base();
        virtual void init() = 0;
        virtual void leave()
        {
            for(int i = 0; i < threads.size(); i++)
                threads[i].join();
        }
        bool AppendGeneral(const Append_info* request, returnAppend* response);
        bool VoteGeneral(const VoteInfo* request, returnVote* response);
        virtual void AppendRPC(const Append_info* request, returnAppend* response) = 0;
        virtual void VotePRC(const VoteInfo* request, returnVote* response) = 0;
    };
    class follower : public identity_base
    {
    public:
        follower();
        void init();
        void AppendRPC(const Append_info* request, returnAppend* response);
        void VotePRC(const VoteInfo* request, returnVote* response);
    };
    class candidate : public identity_base
    {
    public:
        candidate();
        void init();
        void AppendRPC(const Append_info* request, returnAppend* response);
        void VotePRC(const VoteInfo* request, returnVote* response);
    };
    class leader : public identity_base
    {
        leader();
        void init();
        void AppendRPC(const Append_info* request, returnAppend* response);
        void VotePRC(const VoteInfo* request, returnVote* response);
    };
}
#endif //RAFT_YYH_IDENTITY_H
