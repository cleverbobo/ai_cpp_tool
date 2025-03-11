#include <fstream>
#include <sstream>
#include <iostream>

#include "ai_tools.h"


BoxVec load_box_data(const std::string filename) {
    BoxVec boxes; // 用于存储 Box 结构体的向量
    std::ifstream file(filename); // 打开文件

    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return boxes; // 返回空向量
    }

    std::string line;
    while (std::getline(file, line)) { // 逐行读取文件
        std::istringstream iss(line); // 将行内容转换为字符串流
        Box box;
        float left, top, right, bottom, area, score;
        // 读取每个字段，注意根据输入格式调整
        if (iss >> left >> top >> right >> bottom >> area >> score) {
            box.x = (left + right) / 2 ;
            box.y = (top + bottom) /2 ;
            box.width = right-left > 0 ? right-left : 0;
            box.height = bottom - top > 0 ? bottom - top : 0;
            box.score = score;
            boxes.push_back(box); // 将 box 添加到向量中
        } else {
            std::cerr << "读取数据失败，可能是首行label: " << line << std::endl;
        }
    }

    file.close(); // 关闭文件
    std::cout << "load test data success. input data num: " << boxes.size() << "\n";
    return boxes; // 返回 Box 向量
}

bool dump_box_data(const std::string filename, BoxVec& boxes) {
    std::ofstream outFile;

    // 设置异常处理
    outFile.exceptions(std::ofstream::failbit | std::ofstream::badbit);

    try {
        outFile.open(filename);
        // 写入 CSV 表头
        outFile << "x,y,width,height,score,class_id\n";

        // 写入每个 Box 的数据
        for (const auto& box : boxes) {
            outFile << box.x << ','
                    << box.y << ','
                    << box.width << ','
                    << box.height << ','
                    << box.score << ','
                    << box.class_id << '\n';
        }

        outFile.close();
    }
    catch (const std::ofstream::failure& e) {
        std::cerr << "Exception occurred while writing to file: " << e.what() << "\n";
        return false;
    }

    return true;
}

