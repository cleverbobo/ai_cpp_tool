cmake_minimum_required(VERSION 3.10)
project(main)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)

# 设置构建类型为 Release
set(CMAKE_BUILD_TYPE "Release")

# 添加子目录以编译 nms_func
add_subdirectory(NMS/cpp)

# 包含主函数和库的头文件目录
include_directories(./utils/include)
include_directories(./TestCase)
include_directories(NMS/cpp)  # 包含 NMS 的头文件

# 添加可执行文件
file(GLOB SRC_FILES
    "./TestCase/*.cpp"
    "./utils/src/*.cpp"
)

# 添加可执行文件
add_executable(main main.cpp ${SRC_FILES})

# 链接静态库（假设 NMS 库的目标名称为 nms）
target_link_libraries(main nms)  # 这里使用目标名称而不是文件路径
