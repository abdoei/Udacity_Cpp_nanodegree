#pragma once 

using std::string;

class Path{
public:
    static string cmdPath() {return "/cmdline";}
    static string statusPath() {return "/status";}
    static string basePath() {return "/proc/";}
    static string statPath() {return "/stat";}
    static string upTimePath() {return "uptime";}
    static string memInfoPath() {return "meminfo";}
    static string versionPath() {return "version";}
};

enum CPUStates{
    S_USER = 1,
    S_NICE,
    S_SYSTEM,
    S_IDLE,
    S_IOWAIT,
    S_IRQ,
    S_SOFTIRQ,
    S_STEAL,
    S_GUEST,
    S_GUEST_NICE
};
