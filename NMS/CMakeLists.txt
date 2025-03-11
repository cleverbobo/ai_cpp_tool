cmake_minimum_required(VERSION 3.10)
project(NMS)

# 设置C++标准
set(CMAKE_CXX_STANDARD 14)

# 设置构建类型为 Release
set(CMAKE_BUILD_TYPE "Release")
# 设置 Release 模式下的编译选项为 -O3
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -funroll-all-loops -fivopts -flto -march=armv8-a -mtune=cortex-a53 -mcpu=cortex-a53 -fomit-frame-pointer -fstrict-aliasing -ffast-math -DNDEBUG -fno-exceptions -fno-rtti -fno-strict-overflow -floop-parallelize-all -ftree-parallelize-loops=4 -ftree-vectorize -fexceptions")

# 创建静态库
add_library(nms STATIC nms.cpp nms.h)

# 设置静态库输出目录为根目录下的 lib 文件夹
set_target_properties(nms PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
)

# 指定包含目录（如果 `nms.h` 需要被外部使用）
target_include_directories(nms PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 输出构建信息
message(STATUS "Static library created: NMS")


