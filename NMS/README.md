# NMS

## 说明
高性能的单类别NMS，尤其arm-A53上，性能尤其出众。

## 性能测试
测试数据路径为 `Data/bbox.txt` 。

### NMS_CPP 性能测试
|  测试设备        |    耗时(us)    |
|------------------|----------------|
| arm-A53 @1.6 GHz  |   950         |
| i9-10900X @3.7GHz  |   900         |

### NMS_Python-opencv 性能测试
|  测试设备        |    耗时(us)    |
|------------------|----------------|
| arm-A53 @1.6 GHz  |   4767.52         |
| i9-10900X @3.7GHz  |   468.25         |

边缘端部署，性能优势提升明显

## 已知问题
1. loopnum > 1时，耗时测试存在问题，这可能与编译优化选项有关。上述测试数据以loopnum=1测试得到的。