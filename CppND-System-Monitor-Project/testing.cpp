#include "util.hpp"
#include <iostream>
#include <sstream>
#include "ProcessParser.hpp"

using namespace std;

int main(){
    // constants.hpp
    // cout << Util::convertToTime(1000) << endl;
    // cout << Util::getProgressBar("50") << endl;

/////////////////
    cout << '\n';
/////////////////

    auto myfile = Util::getStream("testing.cpp");
    if (myfile)
    {
        string line;
        // while (getline(myfile, line)) cout << line << endl;
    }

/////////////////
    cout << '\n';
/////////////////

    // ProcessParser.hpp
    // cout << ProcessParser::getCmd("1660") << endl;
    // this happens to be valid [when running on my machine] and gives "/usr/libexec/gvfs-udisks2-volume-monitor"

/////////////////
    cout << '\n';
/////////////////

    vector<string> processes = ProcessParser::getPidList();
    // for (string proc : processes)
    //     cout << proc << " | ";

/////////////////
    cout << '\n';
/////////////////

    int pNum = 2144;
    // cout << "Process No." << to_string(pNum) << " got a RAM usage of " << ProcessParser::getVmSize(to_string(pNum)) << " GBytes" << endl; 

/////////////////
    cout << '\n';
/////////////////

    // cout << "Process No." << to_string(pNum) << " uses " << ProcessParser::getCpuPercent(to_string(pNum)) << "\% of the cpu power";

/////////////////
    cout << '\n';
/////////////////

    // cout << "Process No." << to_string(pNum) << " belongs to user: " << ProcessParser::getProcUser(to_string(pNum)) << endl;

/////////////////
    cout << '\n';
/////////////////

    // cout << "No. of cores = " << ProcessParser::getNumberOfCores()<< endl;

/////////////////
    cout << '\n';
/////////////////

    // for (auto& i : ProcessParser::getSysCpuPercent("0"))
    //     cout << i << " | ";

/////////////////
    cout << '\n';
/////////////////

    // cout << "System Ram %: " << ProcessParser::getSysRamPercent() << endl;

/////////////////
    cout << '\n';
/////////////////

    cout << "System kernel version: " << ProcessParser::getSysKernelVersion() << endl;

/////////////////
    cout << '\n';
/////////////////

    cout << "System name: " << ProcessParser::getOsName() << endl;

/////////////////
    cout << '\n';
/////////////////

    cout << "Total threads number: " << ProcessParser::getTotalThreads() << endl;

/////////////////
    cout << '\n';
/////////////////

    cout << "Total processes number: " << ProcessParser::getTotalNumberOfProcesses() << endl;

/////////////////
    cout << '\n';
/////////////////

    cout << "Total running processes number: " << ProcessParser::getNumberOfRunningProcesses() << endl;

    return 0;
}