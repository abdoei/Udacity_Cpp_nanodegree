#pragma once
#include <string>
#include "ProcessParser.hpp"
using std::string;

class Process
{
private:
    string pid;
    string user;
    string cmd;
    string cpu;
    string mem;
    string upTime;
    void setPid(int pid);

public:
    Process(string pid){
        this->pid = pid;
        this->user = ProcessParser::getProcUser(pid);
        this->mem = ProcessParser::getVmSize(pid);
        this->cmd = ProcessParser::getCmd(pid);
        this->upTime = ProcessParser::getProcUpTime(pid);
        this->cpu = ProcessParser::getCpuPercent(pid);
    }
    string getPid() const;
    string getUser() const;
    string getCmd() const;
    float getCpu() const;
    float getMem() const;
    string getUpTime() const;
    string getProcess();
    string formatCMD(string cmd);
};

void Process::setPid(int pid){
    this->pid = pid;
}

string Process::getPid() const{
return this->pid;
}

string Process::getProcess(){
// to return the process attributes in a single string

    // firstly we need to update the process attributes
    this->mem = ProcessParser::getVmSize(this->pid);
    this->upTime = ProcessParser::getProcUpTime(this->pid);
    this->cpu = ProcessParser::getCpuPercent(this->pid);

    // then we can return the process attributes in a single string
    return (this->pid + "   "
            + this->user
            + "   "
            + this->mem.substr(0,5)
            + "     "
            + this->cpu.substr(0,5)
            + "     "
            + this->upTime.substr(0,5)
            + "    "
            + formatCMD( this->cmd)
            );
}

string Process::formatCMD(string cmd) {
    if(cmd.length() <= 40){
        return cmd;
   }else{
        int indeces[20]; // to store the indeces of the / in the cmd string
        int count = 0;
        for(int i = 0; i < cmd.size(); ++i){ if(cmd[i] == '/') indeces[count++] = i;}
        string formattedCMD = "/";
        int itemSpace = (cmd.size() - count) / count;
        for(int i = 1; i < count+1 ; ++i){
            if(indeces[i]-indeces[i-1] <= itemSpace){
                formattedCMD += cmd.substr(indeces[i-1]+1, indeces[i]-indeces[i-1]-1);
            }
            else{formattedCMD += cmd.substr(indeces[i-1]+1, itemSpace);}
            if(i != count) formattedCMD += "/";
        }
        return formattedCMD;
    }
}

string Process::getUser() const{
    return this->user;
}

string Process::getCmd() const{
    return this->cmd;
}

float Process::getCpu() const{
    return stof(this->cpu);
}

float Process::getMem() const{
    return stof(this->mem);
}

string Process::getUpTime() const{
    return this->upTime;
}

