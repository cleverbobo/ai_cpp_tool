#include <algorithm>

#include "nms.h"

// C++ NMS
void NMS(const BoxVec &dets, BoxVec &res, float nmsConfidence) {
    if (dets.empty()) {
        return;
    }

    size_t size = dets.size();
    // 对检测框按照置信度从高到低排序
    std::vector<Box> sorted_dets = dets;
    std::sort(sorted_dets.begin(), sorted_dets.end(), [](const Box &l, const Box &r) {
        return l.score > r.score;
    });

    std::vector<char> pValidIndexes(size, 1); // 使用 char 节省内存
    std::vector<float> pData(5 * size);       // 存储 [x1, y1, x2, y2, area]

    // 预计算每个检测框的坐标和面积
    for (size_t i = 0; i < size; ++i) {
        float x1 = sorted_dets[i].x - sorted_dets[i].width / 2.0f;
        float y1 = sorted_dets[i].y - sorted_dets[i].height / 2.0f;
        float x2 = sorted_dets[i].x + sorted_dets[i].width / 2.0f;
        float y2 = sorted_dets[i].y + sorted_dets[i].height / 2.0f;
        float area = sorted_dets[i].width * sorted_dets[i].height;

        pData[i * 5 + 0] = x1;
        pData[i * 5 + 1] = y1;
        pData[i * 5 + 2] = x2;
        pData[i * 5 + 3] = y2;
        pData[i * 5 + 4] = area;
    }

    // 开始进行 NMS 处理
    for (size_t m = 0; m < size; ++m) {
        if (!pValidIndexes[m]) {
            continue;
        }
        const float *lbox = &pData[m * 5];
        res.push_back(sorted_dets[m]);

        for (size_t n = m + 1; n < size; ++n) {
            if (!pValidIndexes[n]) {
                continue;
            }

            const float *rbox = &pData[n * 5];

            // 计算交叉区域
            float inter_x1 = std::max(lbox[0], rbox[0]);
            float inter_y1 = std::max(lbox[1], rbox[1]);
            float inter_x2 = std::min(lbox[2], rbox[2]);
            float inter_y2 = std::min(lbox[3], rbox[3]);

            if (inter_x1 >= inter_x2 || inter_y1 >= inter_y2) {
                continue; // 没有重叠区域
            }

            float inter_area = (inter_x2 - inter_x1) * (inter_y2 - inter_y1);
            float union_area = lbox[4] + rbox[4] - inter_area;
            float iou = inter_area / union_area;

            if (iou > nmsConfidence) {
                pValidIndexes[n] = 0; // 抑制掉重叠程度较高的框
            }
        }
    }
}
