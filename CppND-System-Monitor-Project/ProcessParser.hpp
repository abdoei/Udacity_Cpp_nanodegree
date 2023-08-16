/* 
// The required header files given by the course.
#include <algorithm> //
#include <iostream> //
#include <math.h>
#include <thread>
#include <chrono>
#include <iterator>
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
#include <unistd.h>
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

using std::string;
using std::vector;

class ProcessParser {
    public:
        static std::string getCmd(std::string pid);
        static std::vector<std::string> getPidList();
        static std::string getVmSize(std::string pid);
        static std::string getCpuPercent(std::string pid);
        static long int getSysUpTime();
        static std::string getProcUpTime(std::string pid);
        static std::string getProcUser(std::string pid);
        static std::vector<std::string> getSysCpuPercent(std::string coreNumber = "");
        static float getSysRamPercent();
        static std::string getSysKernelVersion();
        static int getTotalThreads();
        static int getTotalNumberOfProcesses();
        static int getNumberOfRunningProcesses();
        static string getOsName();
        static std::string printCpuStats(std::vector<std::string> values1, std::vector<std::string>values2);
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
            string strVmSize = line.substr(searchString.size(), line.size());
            return std::to_string(std::stoi(strVmSize) / 1024.0 / 1024.0 );
        }
    }
    return string("-1");
}
