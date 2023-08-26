#pragma once
#include <string>
#include <fstream>

using std::string;

// Classic helper functions
class Util final {
    public:
        static string convertToTime ( long int input_seconds );
        static string getProgressBar(string percent);
        static std::ifstream getStream(string path);
};

string Util::convertToTime (long input_seconds)
{
    if(input_seconds < 0) throw std::runtime_error("Negative system time");
    long minutes = input_seconds / 60;
    long hours = minutes / 60;
    long seconds = int(input_seconds%60);
    minutes = int(minutes%60);
    long days = hours / 24;
    hours = hours % 24;
    string result = std::to_string(days) + " days, " + std::to_string(hours) +
                    ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
    return result;
}

// constructing string for given percentage
// 50 bars is uniformly streched 0 - 100 %
// meaning: every 2% is one bar(|)
string Util::getProgressBar(string percent)
{
    string result = "0% ";
    int _size= 50;
    int  boundaries = (stof(percent)/100)*_size;

    for (int i=0;i<_size;++i) {
        if (i<=boundaries) result +="|";
        else               result +=" ";
    }

    result +=" " + percent.substr(0,5) + " /100%";
    return result;
}

// wrapper for creating streams
std::ifstream Util::getStream(string path)
{
    std::ifstream stream(path);
    if (!stream) throw std::runtime_error("Non-existing PID");

    return stream;
}