#ifndef RAFT_YYH_LOG_H
#define RAFT_YYH_LOG_H

#include <string>
#include <cstdint>
namespace yyh_raft {

    class log {
    public:
        enum operation {
            Require, Modify
        };
        operation type;
        int64_t idx;
        int64_t opp;
        int64_t Term;
        log(std::string ss);
    };
}

#endif //RAFT_YYH_LOG_H
