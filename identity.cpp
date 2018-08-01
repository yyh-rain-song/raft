#include "identity.h"
#define debug
using namespace yyh_raft;
/*
identity_base::identity_base() {

}

follower::follower() {
    Type = Follower;
    elecTimeout = 1500;
    elecTurn = 0;
    dect_timeout.timeout = false;
    Time = clock();
}
void follower::detectTime()
{
    std::cout << "counting time" << '\n';
    if((clock() - Time) < elecTimeout)
        std::this_thread::sleep_for(std::chrono::milliseconds(elecTimeout - (clock() - Time)));
    while((clock() - Time) < elecTimeout)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::unique_lock<std::mutex> guard(dect_timeout.lock_);
    dect_timeout.timeout = true;
    dect_timeout.lock_cond.notify_one();
    std::cout << "lose heart beat" << '\n';
}
void follower::detectHeart()
{
    std::cout << "I'm hearing heart beat" << '\n';
    //this thread listens to the heartbead from leader or candidate and clear elecTimeout
}
void follower::identity_transfer()
{
    std::thread watch_time(&follower::detectTime, this);
    watch_time.detach();
    std::thread listen_heart(&follower::detectHeart, this);
    listen_heart.detach();
    std::cout << "id transfer sleep......" << '\n';
    std::unique_lock<std::mutex> guard(dect_timeout.lock_);
    dect_timeout.lock_cond.wait(guard,[this]{ return dect_timeout.timeout;});
    std::cout << "id transfer awake" << '\n';
    Type = Candidate;
    ++elecTurn;
    std::cout << "ID transfermed" << '\n';
}

void candidate::identity_transfer() {

}

void candidate::detectTime() {

}

void candidate::detectHeart() {
    std::cout << "I'm listening" << '\n';
    //this thread listens vote or heartbeat from follower/candidate
}

candidate::candidate() {
    Type = Candidate;
    elecTimeout = 0;
    elecTurn = 0;
    dect_timeout.timeout = false;
    Time = clock();
}
*/


identity_base::identity_base() {

}

follower::follower() {
    currentTerm = 0;
    init();
}

void follower::init() {
    Time.restart();
    type = Follower;
    elecTimeout = (int32_t)intRand(150, 300);
    currentTerm++;
    votedThisTurn = false;
}

bool identity_base::AppendGeneral(const Append_info *request, returnAppend *response)
{
    //判断是否可以接受
    bool accept = true;
    {
        if(request->term() < currentTerm) accept = false;
        if(request->term() == currentTerm && type == Leader) accept = false;
        int64_t pret = stored_logs[request->prevlogindex()].Term;
        if(pret != request->prevlogterm()) accept = false;
    }
    //返回不接受
    if(!accept)
    {
        response->set_term(currentTerm);
        response->set_success(false);
    }
    else
    {
        std::string entry = request->entry();
        //更新日志
        if (entry.size() != 0) {
            yyh_raft::log news_one(entry);
            //删除冲突日志
            if (stored_logs[news_one.idx].Term != news_one.Term)
            {
                for (int64_t i = news_one.idx; i < stored_logs.size(); i++)
                    stored_logs.pop_back();
            }
            //附加新日志
            stored_logs[news_one.idx] = news_one;
            if (request->leadercommit() > commitIndex)
                commitIndex = std::min(request->leadercommit(), news_one.idx);
        }
        //返回成功
        response->set_term(currentTerm);
        response->set_success(true);
    }
    //是否身份转换
    return (accept && request->term() > currentTerm);
}

bool identity_base::VoteGeneral(const VoteInfo *request, returnVote *response) {
    //判断接受
    bool accept = true;
    {
        if(request->term() < currentTerm) accept = false;
        if(votedThisTurn) accept = false;
        //日志新旧问题
    }
    //投票
    if(accept)
        votedThisTurn = true;
    response->set_term(currentTerm);
    response->set_votegranted(accept);
    //是否身份转换
    return (accept && request->term() > currentTerm);
}

void identity_base::debug_out() {
#ifdef  debug
    printf("ID:%s, type:%d, votedFor:%d currentTerm:%d\n", ID, type, votedFor, currentTerm);
#endif
}
