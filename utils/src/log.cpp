#include <cstring>
#include <iostream>

#include "log.h"


// time function
void TimeStamp::ShowInfo() {
    std::cout << "================================ Time INFO ===============================\n";
    for (auto iter = TimeMap.begin(); iter != TimeMap.end(); iter++) {
        if (iter->second.size() % 2 != 0) {
            std::cout << iter->first << " record error! (odd number of timestamps)\n";
            continue;
        } else {
            std::chrono::microseconds totalDuration(0);
            size_t count = iter->second.size() / 2; // Number of pairs
            for (size_t i = 0; i < count; i++) {
                totalDuration += std::chrono::duration_cast<std::chrono::microseconds>(iter->second[2 * i + 1] - iter->second[2 * i]);
            }

            std::cout << iter->first << ": " << totalDuration.count() / static_cast<double>(count) << "us \n";
        }
    }
    std::cout << "================================ Time INFO ===============================\n";
}

void TimeStamp::RecordTime(const std::string TimeName){
    auto Time = std::chrono::high_resolution_clock::now();
    TimeMap[TimeName].push_back(Time);
}

