/* 
// The required header files given by the course.
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
        static std::string printCpuStats(std::vector<std::string> values1, std::vector<std::string>values2);
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
    std::istream_iterator<string> beg(buf), end;
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
            std::vector<std::string> words(std::istream_iterator<std::string>{iss},
                                           std::istream_iterator<std::string>());
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

int ProcessParser::getNumberOfCores()
{
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

vector<string> ProcessParser::getSysCpuPercent(std::string coreNumber){
    string path = Path::basePath() + Path::statPath();
    ifstream stream = Util::getStream(path);
    string line;
    string searchString = "cpu" + coreNumber;
    
    while (getline(stream, line))
    {
        if(line.compare(0, searchString.size(), searchString) == 0){
            istringstream iss{line};
            vector<string> data(++(istream_iterator<std::string>{iss}),
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


string ProcessParser::printCpuStats(vector<string> values1, vector<string> values2){
/*
Because CPU stats can be calculated only if you take measures in two different time,
this function has two parameters: two vectors of relevant values.
We use a formula to calculate overall activity of processor.
*/
    float activeTime = getSysActiveCpuTime(values2) - getSysActiveCpuTime(values1);
    float idleTime = getSysIdleCpuTime(values2) - getSysIdleCpuTime(values1);
    float totalTime = activeTime + idleTime;
    float result = 100.0*(activeTime / totalTime);
    return to_string(result);
}