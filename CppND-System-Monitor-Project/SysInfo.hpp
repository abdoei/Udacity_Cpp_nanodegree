#pragma once
#include <vector>
#include <string>
#include "ProcessParser.hpp"
#include "util.hpp"

using std::vector;
using std::string;

class SysInfo {
    private:
        vector<string> lastCpuStats;     // cpu stats like: cpu 10132153 290696 3084719 46828483 16683 0 25195 0
        vector<string> currentCpuStats;  // the current version of the lastCpuStats [for the relative calculations]

        // the % percentage of the load
        vector<string> cpuCoresStats;                 // the stats displayed as % percentages only
        vector<vector<string>> currentCpuCoresStats;  // the stats currently processed like:cpu1 108506 5 33364 1279573 1905 0 175 0 0 0 for each core
        vector<vector<string>> lastCpuCoresStats;     // the stats of the past cycle [for the relative calculations]

        string cpuPercent;
        string osName;
        string kernelVer;
        float memPercent;
        long upTime;
        int totalProc;
        int runningProc;
        int threads;

    public:
        SysInfo() {
            /*
            Getting initial info about system
            Initial data for individual cores is set
            System data is set
            */
            this->getOtherCores(ProcessParser::getNumberOfCores());
            this->setLastCpuMeasures();
            // this->setCpuCoresStats();
            this->setAttributes();
            this->osName = ProcessParser::getOsName();
            this->kernelVer = ProcessParser::getSysKernelVersion();
        }

        // getters
        long   getUpTime()        const;
        string getCpuPercent()    const; // total CPU usage percentage
        string getKernelVersion() const;
        string getOsName()        const;
        string getTotalProc()     const; // total processes count
        string getRunningProc()   const; // total running processes count
        string getThreads()       const; // total threads count
        string getMemPercent()    const; // memory usage percentage
        vector<string> getCoresStats() const; // cpu0: 0% |||||       10% /100%
        void   getOtherCores(int _size);

        // setters
        void   setLastCpuMeasures();
        void   setAttributes();
        void   setCpuCoresStats();
};

// Getters definitions
long   SysInfo::getUpTime()        const { return this->upTime;     }
string SysInfo::getCpuPercent()    const { return this->cpuPercent; }
string SysInfo::getKernelVersion() const { return this->kernelVer;  }
string SysInfo::getOsName()        const { return this->osName;     }
string SysInfo::getTotalProc()     const { return to_string(this->totalProc);  }
string SysInfo::getRunningProc()   const { return to_string(this->runningProc);}
string SysInfo::getThreads()       const { return to_string(this->threads);    }
string SysInfo::getMemPercent()    const { return to_string(this->memPercent); }

// Call the get before this get or set the coresStats variable to always hit the first time
vector<string> SysInfo::getCoresStats() const {
    /*
    * This method is meant to return the last
    * output to be printed like:
    * cpu0: 0% |||||       10% /100%
    * cpu1: 0% ||||        8%  /100%
    * cpu2: 0% ||||||      12% /100%
    * cpu3: 0% |||         6%  /100%
    */
    vector<string> stats;
    for (int i = 0; i < (this->cpuCoresStats.size()); ++i) {
        string line = "cpu" + to_string(i) + ": " + Util::getProgressBar(this->cpuCoresStats[i]);
        stats.emplace_back(line);
    }
    return stats;
}

void SysInfo::getOtherCores(int _size) {
// It takes a size, or number of cores, and initializes the object.
    this->cpuCoresStats        = vector<string>();
    this->lastCpuCoresStats    = vector<vector<string>>();
    this->currentCpuCoresStats = vector<vector<string>>();

    this->       cpuCoresStats.resize(_size);
    this->   lastCpuCoresStats.resize(_size);
    this->currentCpuCoresStats.resize(_size);

    for(int i = 0; i < _size; ++i)
        this->lastCpuCoresStats[i] = ProcessParser::getSysCpuPercent(to_string(i));
        // it gives back the stats of the ith core
}


// Setter definitions
void SysInfo::setLastCpuMeasures() {
    // set the currently read cpu stats to the last cpus stats vector<string>
    this->lastCpuStats = ProcessParser::getSysCpuPercent();
}

void SysInfo::setCpuCoresStats() {
    // Getting data from files (previous data is required)
    for(int i = 0; i < (this->currentCpuCoresStats.size()); ++i)
        this->currentCpuCoresStats[i] = ProcessParser::getSysCpuPercent(to_string(i));

    // after acquirement of data we are calculating every core percentage of usage
    for(int i = 0; i < (this->currentCpuCoresStats.size()); ++i)
        this->cpuCoresStats[i] = ProcessParser::printCpuStats(this->lastCpuCoresStats[i],this->currentCpuCoresStats[i]);

    this->lastCpuCoresStats = this->currentCpuCoresStats;
}

void SysInfo::setAttributes() {
        // this->osName          = ProcessParser::getOsName();
        // this->upTime          = ProcessParser::getSysUpTime();
        // this->threads         = ProcessParser::getTotalThreads();
        // this->memPercent      = ProcessParser::getSysRamPercent();
        // this->kernelVer       = ProcessParser::getSysKernelVersion();
        // this->totalProc       = ProcessParser::getTotalNumberOfProcesses();
        // this->runningProc     = ProcessParser::getNumberOfRunningProcesses();

        this->memPercent = ProcessParser::getSysRamPercent();
        this->upTime = ProcessParser::getSysUpTime();
        this->totalProc = ProcessParser::getTotalNumberOfProcesses();
        this->runningProc = ProcessParser::getNumberOfRunningProcesses();
        this->threads = ProcessParser::getTotalThreads();

        this->currentCpuStats = ProcessParser::getSysCpuPercent();
        this->cpuPercent      = ProcessParser::printCpuStats(this->lastCpuStats, this->currentCpuStats);
        this->lastCpuStats    = this->currentCpuStats;

        this->setCpuCoresStats();
}
