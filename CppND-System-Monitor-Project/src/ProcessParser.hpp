/*
The required header files given by the course.
#include <algorithm> //
#include <iostream> //
#include <math.h>
#include <thread>
#include <chrono>
#include <iterator> //
#include <string> //
#include <stdlib.h>
#include <stdio.h>
#include <vector> //
#include <fstream> //
#include <sstream> //
#include <stdexcept>
#include <cerrno> //
#include <cstring> //
#include <dirent.h> //
#include <time.h>
#include <unistd.h> //
#include <constants.h> //
*/
#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "constants.hpp"
#include <fstream>
#include <dirent.h>
#include <cerrno>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <unistd.h>
#include "util.hpp"

using std::string;
using std::vector;
using std::ifstream;
using std::istringstream;
using std::to_string;
using std::istream_iterator;

class ProcessParser {
    public:
        static std::string getCmd(std::string pid);
        static std::vector<std::string> getPidList();
        static std::string getVmSize(std::string pid);
        static std::string getCpuPercent(std::string pid);
        static long int getSysUpTime();
        static std::string getProcUpTime(std::string pid);
        static std::string getProcUser(std::string pid);
        static std::vector<std::string> getSysCpuPercent(std::string coreNumber);
        static float getSysRamPercent();
        static std::string getSysKernelVersion();
        static int getTotalThreads();
        static int getTotalNumberOfProcesses();
        static int getNumberOfRunningProcesses();
        static string getOsName();
        static std::string printCpuStats(std::vector<std::string> lastValues, std::vector<std::string>currentValues);
        static int getNumberOfCores();
        static float getSysActiveCpuTime(vector<string> values);
        static float getSysIdleCpuTime(vector<string> values);
};

string ProcessParser::getCmd(string pid){
    string cmdline;
    string path = Path::basePath() + pid + Path::cmdPath();
    std::ifstream stream = Util::getStream(path);
    std::getline(stream, cmdline);

    return cmdline;
}

vector<string> ProcessParser::getPidList(){
    vector<string> pidList;
    DIR* directory;
    dirent *entery;

    directory = opendir(Path::basePath().c_str());
    if(directory){
        while ((entery = readdir(directory)))
        {
            if(entery->d_type != DT_DIR) continue;             // if it is not a direcotry
            if(std::all_of(entery->d_name, entery->d_name + strlen(entery->d_name), [](const char c){return isdigit(c);}))
                pidList.emplace_back(entery->d_name); // if all of its name chars are digits
        }
        if(closedir(directory) == -1) // If not closed safely it returns -1
            throw std::runtime_error(std::strerror(errno));
    }
    else
        throw std::runtime_error(std::strerror(errno));

    return pidList;
}

string ProcessParser::getVmSize(string pid){
    string path = Path::basePath() + pid + Path::statusPath();
    string searchString = "VmData:";
    std::ifstream stream = Util::getStream(path);
    string line;
    while(getline(stream, line)){
        if(line.compare(0, searchString.size(), searchString) == 0){
            string strVmSize = line.substr(searchString.size(), line.size()); // redundant
            return std::to_string(std::stoi(strVmSize) / 1024.0 / 1024.0 );
        }
    }
    return string("-1");
}

string ProcessParser::getCpuPercent(string pid){
    int sTime_i = 14, cuTime_i = 15, csTime_i = 16, startTime_i = 21;
    ifstream stream = Util::getStream(Path::basePath() + pid + Path::statPath());
    string line;
    getline(stream, line);
    istringstream buf(line);
    istream_iterator<string> beg(buf), end;
    vector<string> times (beg, end);
    /*
    utime: user mode jiffies
    stime: kernel mode jiffies
    cutime: user mode jiffies with child's
    cstime: kernel mode jiffies with child's
    start_time: time the process started after system boot

    jiffies: An incrementing counter representing
    system "uptime" in ticks - or the number of
    timer interrupts since boot. Ultimately the
    entire original concept of a jiffy will likely
    vanish as systems use timer events only when
    necessary and become "jiffyless".

    */
    float sTime = stof(times[sTime_i]);
    float cuTime = stof(times[cuTime_i]);
    float csTime = stof(times[csTime_i]);
    float startTime = stof(times[startTime_i]);
    float freq = sysconf(_SC_CLK_TCK);
    // not imp
    float uTime = stof(ProcessParser::getProcUpTime(pid));
    float upTime = ProcessParser::getSysUpTime();

    // calculations
    float total_time = uTime + sTime + cuTime + csTime;
    float seconds = upTime - (startTime / freq); // time elapsed from the begining of the process to now
    float result = 100.0*((total_time / freq) / seconds);
    return to_string(result);
}

string ProcessParser::getProcUpTime(string pid){
    ifstream stream = Util::getStream(Path::basePath() + pid + Path::statPath());
    string line;
    getline(stream, line);
    istringstream buf(line);
    istream_iterator<string> beg(buf), end;
    vector<string> stats(beg, end);
    return to_string(stof(stats[13]) / sysconf(_SC_CLK_TCK));
}

long int ProcessParser::getSysUpTime(){
    ifstream stream = Util::getStream(Path::basePath() + Path::upTimePath());
    string line;
    getline(stream, line);
    istringstream buf(line);
    istream_iterator<string> beg(buf), end;
    vector<string> values (beg, end);
    return stoi(values[0]);
}

string ProcessParser::getProcUser(string pid){
    string path = Path::basePath() + pid + Path::statusPath();
    string searchString = "Uid:";
    ifstream stream = Util::getStream(path);
    string line;
    string Uid;
    while(getline(stream, line)){
        if(line.compare(0, searchString.size(), searchString) == 0){
            std::istringstream iss(line);
            std::vector<std::string> words(istream_iterator<std::string>{iss},
                                           istream_iterator<std::string>());
            Uid = words[1];
            break;
        }
    }
    stream = Util::getStream("/etc/passwd");
    searchString = ":x:" + Uid;
    int firstColonPos;
    while(getline(stream, line)){
        if((firstColonPos = line.find(searchString)) != string::npos){
            Uid = line.substr(0, firstColonPos);
            break;
        }
    }

    return Uid;
}

int ProcessParser::getNumberOfCores(){
    // Get the number of host cpu cores
    string line;
    string name = "cpu cores";
    ifstream stream = Util::getStream((Path::basePath() + "cpuinfo"));
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(),name) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            return stoi(values[3]);
        }
    }
    return 0;
}

// TODO: suggest a name chage
vector<string> ProcessParser::getSysCpuPercent(std::string coreNumber = ""){
    /*
     *  The amount of time that the system ("cpu" line) or
     *  the specific CPU ("cpuN" line) spent in various states like:
     *  cpu  10132153 290696 3084719 46828483 16683 0 25195 0 175628 0
     *  cpu0 1393280  32966  572056  13343292 6130  0 17875 0 23933  0
     *
     *  The meanings of the columns are as follows, from left to right:
     *      user:    normal processes executing in user mode
     *      nice:    niced processes executing in user mode
     *      system:  processes executing in kernel mode
     *      idle:    twiddling thumbs
     *      iowait:  waiting for I/O to complete
     *      irq:     servicing interrupts
     *      softirq: servicing softirqs
     *      steal:   time spent in other operating systems like virtualized environment
     *      guest:   stolen time for guest OS under the control of the Linux kernel
     *      guest_nice: Time spent running a niced guest
     */
    string path = Path::basePath() + Path::statPath();
    ifstream stream = Util::getStream(path);
    string line;
    string searchString = "cpu" + coreNumber;

    while (getline(stream, line))
    {
        if(line.compare(0, searchString.size(), searchString) == 0){
            istringstream iss{line};
            vector<string> data(istream_iterator<std::string>{iss},
                                istream_iterator<std::string>()); // skiping the first element
            return data;
        }
    }
    return vector<string>();
}

// Values 1:3, 6:10 are active CPU time
float ProcessParser::getSysActiveCpuTime(vector<string> values){
    return (stof(values[S_USER])    +
            stof(values[S_NICE])    +
            stof(values[S_SYSTEM])  +
            stof(values[S_IRQ])     +
            stof(values[S_SOFTIRQ]) +
            stof(values[S_STEAL])   +
            stof(values[S_GUEST])   +
            stof(values[S_GUEST_NICE]));
}

// Values 4, 5 are idle CPU time
float ProcessParser::getSysIdleCpuTime(vector<string> values) {
    return (stof(values[S_IDLE]) + stof(values[S_IOWAIT]));
}

// lastValues and currentValues are previous and current time. // TODO: suggest a name change
string ProcessParser::printCpuStats(vector<string> lastValues, vector<string> currentValues){
/*
Because CPU stats can be calculated only if you take measures in two different time,
this function has two parameters: two vectors of relevant values.
We use a formula to calculate overall activity of processor.
*/
    float activeTime = getSysActiveCpuTime(currentValues) - getSysActiveCpuTime(lastValues);
    float idleTime   = getSysIdleCpuTime(currentValues) - getSysIdleCpuTime(lastValues);
    float totalTime = activeTime + idleTime;
    float result = 100.0*(activeTime / totalTime);
    return to_string(result);
}

float ProcessParser::getSysRamPercent(){
    string MemAvailable = "MemAvailable:";
    string MemFree = "MemFree:";
    string Buffers = "Buffers:";
    string line;
    ifstream stream = Util::getStream(Path::basePath() + Path::memInfoPath());
    while (getline(stream, line)) {
        if(line.compare(0, MemAvailable.size(), MemAvailable) == 0) MemAvailable = line;
        else if(line.compare(0, MemFree.size(), MemFree) == 0)      MemFree = line;
        else if(line.compare(0, Buffers.size(), Buffers) == 0)      Buffers = line;
    }
    vector<string *> addresses {&MemAvailable, &MemFree, &Buffers};
    for (short i = 0; i < 3; ++i){
        istringstream iss(*addresses[i]);
        vector<string> words(istream_iterator<string>{iss}, istream_iterator<string>());
        *addresses[i] = words[1];
    }
    float output = 100.0 * (1 - (stof(MemFree) / (stof(MemAvailable) - stof(Buffers))));
    return output;
}

string ProcessParser::getSysKernelVersion(){
    string line;
    string searchString = "Linux version ";
    ifstream stream = Util::getStream((Path::basePath() + Path::versionPath()));
    getline(stream, line);
    istringstream buf(line);
    vector<string> values(istream_iterator<string>{buf}, istream_iterator<string>());
    return values[2];
}

string ProcessParser::getOsName(){
    string line;
    string searchString = "PRETTY_NAME=";
    ifstream stream = Util::getStream(("/etc/os-release"));
    while (getline(stream, line)) {
        if(line.compare(0, searchString.size(), searchString) == 0) {
            searchString = line.substr(line.find("\""), line.size());
            break;
        }
    }
    return searchString;
}

int ProcessParser::getTotalThreads() {
    string line;
    string searchString = "Threads:";
    vector<string> pids = getPidList();
    int threadsCounter = 0;
    for(string pid: pids){
        ifstream stream = Util::getStream(Path::basePath() + pid + Path::statusPath());
        while(getline(stream, line)){
            if (line.compare(0, searchString.size(), searchString) == 0) {
                istringstream buf(line);
                vector<string> values(istream_iterator<string>{buf}, istream_iterator<string>());
                threadsCounter += stoi(values[1]);
                break;
            }
        }
    }
    return threadsCounter;
}

int ProcessParser::getTotalNumberOfProcesses(){
    string line;
    int processesCounter = 0;
    string searchString = "processes";
    ifstream stream = Util::getStream((Path::basePath() + Path::statPath()));
    while (std::getline(stream, line)) {
        if (line.compare(0, searchString.size(), searchString) == 0) {
            istringstream buf(line);
            vector<string> values(istream_iterator<string>{buf}, istream_iterator<string>());
            processesCounter += stoi(values[1]);
            break;
        }
    }
    return processesCounter;
}

int ProcessParser::getNumberOfRunningProcesses(){
    string line;
    int runningProcessesCounter = 0;
    string searchString = "procs_running";
    ifstream stream = Util::getStream((Path::basePath() + Path::statPath()));
    while (std::getline(stream, line)) {
        if (line.compare(0, searchString.size(), searchString) == 0) {
            istringstream buf(line);
            vector<string> values(istream_iterator<string>{buf}, istream_iterator<string>());
            runningProcessesCounter += stoi(values[1]);
            break;
        }
    }
    return runningProcessesCounter;
}
