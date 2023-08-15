#include "util.hpp"
#include <iostream>
#include <sstream>

using namespace std;

int main(){
    cout << Util::convertToTime(1000) << endl;
    cout << Util::getProgressBar("50") << endl;
    
    auto myfile = Util::getStream("testing.cpp");
    if (myfile)
    {
        string line;
        while (getline(myfile, line)) cout << line << endl;
    }
    
    return 0;
}