cmake_minimum_required(VERSION 3.10)
project(NMS)

# 设置C++标准
set(CMAKE_CXX_STANDARD 14)

# 设置构建类型为 Release
set(CMAKE_BUILD_TYPE "Release")
# 设置 Release 模式下的编译选项为 -O3
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# 初始化一个变量用于存储优化标志
set(OPTIMIZATION_FLAGS "")

# 检测目标体系结构
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64|armv8)$")
    # 针对ARM架构（如ARM-A53）
    set(OPTIMIZATION_FLAGS "-Ofast -funroll-all-loops -fivopts -flto \
                           -march=armv8-a -mtune=cortex-a53 -mcpu=cortex-a53 \
                           -fomit-frame-pointer -fstrict-aliasing -ffast-math \
                           -DNDEBUG -fno-exceptions -fno-rtti \
                           -fno-strict-overflow -floop-parallelize-all \
                           -ftree-parallelize-loops=4 -ftree-vectorize")
    message(STATUS "Detected ARM architecture. Applying ARM-specific optimization flags.")
else()
    # 默认优化标志（可根据需要调整）
    set(OPTIMIZATION_FLAGS "-O3 -flto -march=native -mtune=native \
                           -fomit-frame-pointer -fstrict-aliasing -ffast-math \
                           -DNDEBUG -fno-exceptions -fno-rtti")
    message(STATUS "Unknown architecture. Applying default optimization flags.")
endif()

# 将优化标志应用到Release构建类型
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPTIMIZATION_FLAGS}")
message(STATUS "[NMS]: CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")


# set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -funroll-all-loops -fivopts -flto -march=armv8-a -mtune=cortex-a53 -mcpu=cortex-a53 -fomit-frame-pointer -fstrict-aliasing -ffast-math -DNDEBUG -fno-exceptions -fno-rtti -fno-strict-overflow -floop-parallelize-all -ftree-parallelize-loops=4 -ftree-vectorize -fexceptions")
include_directories(${CMAKE_SOURCE_DIR}/utils/include)

# 创建静态库
add_library(nms STATIC nms.cpp)
target_include_directories(nms PUBLIC include)

# 设置静态库输出目录为根目录下的 lib 文件夹
set_target_properties(nms PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
)

# 指定包含目录（如果 `nms.h` 需要被外部使用）
target_include_directories(nms PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 输出构建信息
message(STATUS "Static library created: NMS")


