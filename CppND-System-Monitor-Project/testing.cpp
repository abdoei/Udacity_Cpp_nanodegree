#include "util.hpp"
#include <iostream>
#include <sstream>
#include "ProcessParser.hpp"

using namespace std;

int main(){
    // constants.hpp
    cout << Util::convertToTime(1000) << endl;
    cout << Util::getProgressBar("50") << endl;

/////////////////
    cout << '\n';
/////////////////

    auto myfile = Util::getStream("testing.cpp");
    if (myfile)
    {
        string line;
        while (getline(myfile, line)) cout << line << endl;
    }

/////////////////
    cout << '\n';
/////////////////

    // ProcessParser.hpp
    cout << ProcessParser::getCmd("1660") << endl;
    // this happens to be valid [when running on my machine] and gives "/usr/libexec/gvfs-udisks2-volume-monitor"

/////////////////
    cout << '\n';
/////////////////

    vector<string> processes = ProcessParser::getPidList();
    for (string proc : processes)
        cout << proc << " | ";
    
    return 0;
}