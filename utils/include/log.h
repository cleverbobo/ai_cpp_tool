#ifndef LOG_H    
#define LOG_H


#include <chrono>
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>


using timePoint = std::chrono::_V2::system_clock::time_point;

class TimeStamp{
    public:
        void ShowInfo();
        void RecordTime(const std::string TimeName);
    private:
        std::unordered_map<std::string, std::vector<timePoint>> TimeMap;
};




#endif