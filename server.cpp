//
// Created by 12795 on 2018/7/26.
//
#include <string>
class serverInfo
{
public:
    serverInfo();
    void load(std::string &filename);
private:
    std::string serverId;
    std::uint64_t electionTimeout;
};
class server
{
public:

private:
    serverInfo Information;

};
