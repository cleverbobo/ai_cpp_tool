#ifndef AI_TOOLS_H
#define AI_TOOLS_H

#include <vector>
#include <string>

// Detect algorithm 
struct Box {
    float x, y, width, height;
    float score;
    int class_id=0;
};
  
using BoxVec = std::vector<Box>;


BoxVec load_box_data(std::string inputDataPath);

bool dump_box_data(const std::string filename, BoxVec& boxes);


#endif