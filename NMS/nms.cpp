#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "nms.h"




// C++ NMS
void NMS(const BoxVec &dets, BoxVec &res, float nmsConfidence, float iou_thresh) {
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

            if (iou > iou_thresh) {
                pValidIndexes[n] = 0; // 抑制掉重叠程度较高的框
            }
        }
    }
}


// Compile NMS
void NMS_Compile(BoxVec &dets, BoxVec& res,float nmsConfidence, float iou_thresh){
	if (dets.size() == 0) {
        return;
    }
    int size = dets.size();
    std::stable_sort(dets.begin(), dets.end(), [=](const Box& l, const Box& r){return l.score > r.score;});

    bool* pValidIndexes = new bool[size];
    memset(pValidIndexes, 1, size);
    float* pData = new float[5*size];

    for (int i = 0; i < size; i++) {
        pData[i*5+0] = dets[i].x - dets[i].width/2; // x1
        pData[i*5+1] = dets[i].y - dets[i].height/2; // y1
        pData[i*5+2] = dets[i].x + dets[i].width/2; // x2
        pData[i*5+3] = dets[i].y + dets[i].height/2; // y2
        pData[i*5+4] = dets[i].width * dets[i].height;
    }
    int count = 0;
    
    for (int m = 0; m < size; m++) {
        if (!pValidIndexes[m]) continue;
        res.push_back(dets[m]);
        int n = m + 1;
        // 开始内联汇编
        asm (

            "mov x6, %[m_reg]\n"
            "mov x7, %[n_reg]\n"
            "mov x8, %[size_reg]\n"
            // "mov x16, %[pData]\n" 
            
            // 比较 n 和 size
            "cmp x7, x8\n"
            "bge 2f\n"
            "1:\n"
            // 加载 pValidIndexes[n]
            "ldrb w9, [%[pValidIndexes], x7]\n"
            "cbz w9, 3f\n"
    
            // 计算 x10 = x6 * 20
            "lsl x10, x6, #4\n"           // x10 = x6 << 4 (x6 * 16)
            "add x10, x10, x6, lsl #2\n"  // x10 += x6 << 2 (x6 * 4)，总共 x6 * 20
            "add x10, %[pData], x10\n"    // x10 = pData + x10

            // 计算 x11 = x7 * 20
            "lsl x11, x7, #4\n"           // x11 = x7 << 4 (x7 * 16)
            "add x11, x11, x7, lsl #2\n"  // x11 += x7 << 2 (x7 * 4)，总共 x7 * 20
            "add x11, %[pData], x11\n"    // x11 = pData + x11
    
            // 加载 lbox 和 rbox 的坐标
            "ld1 {v0.4s}, [x10]\n"
            "ld1 {v1.4s}, [x11]\n"

            // 计算 interBox 的坐标
            "fmax v2.2s, v0.2s, v1.2s\n"  // interBox x1, y1            
            "fmin v3.4s, v0.4s, v1.4s\n"  // interBox x2, y2
            "mov  v3.s[0], v3.s[2]\n"
            "mov  v3.s[1], v3.s[3]\n"

            // 检查是否有重叠
            "fcmge v4.2s, v2.2s, v3.2s\n"  // 比较 interBox[0] >= interBox \[2], interBox[1] >= interBox[3]
            "mov w13, v4.s[0]\n"
            "mov w14, v4.s[1]\n"
            "orr w15, w13, w14\n"
            "cbnz w15, 3f\n"           // 如果没有重叠，跳过本次循环

            // 计算 inter_wh = interBox max - interBox min
            "fsub v5.2s, v3.2s, v2.2s\n"

            // 将浮点立即数 1.0 加载到向量寄存器 v6.2s
            "fmov v6.2s, #1.0\n"

            // inter_wh += 1.0f
            "fadd v5.2s, v5.2s, v6.2s\n"

            // 计算 interArea = inter_w * inter_h
            // 直接在向量寄存器中完成乘法，并使用 fmul 和 faddp 进行水平加总
            "mov s6, v5.s[0]\n"
            "mov s5, v5.s[1]\n"
            "fmul s0, s5, s6\n"  


            // 加载 lbox_area 和 rbox_area
            "ldr s1, [x10, #16]\n"        // lbox_area
            "ldr s2, [x11, #16]\n"        // rbox_area

            // 计算 unionArea = lbox_area + rbox_area - interArea
            "fadd s3, s1, s2\n"
            "fsub s3, s3, s0\n"

            // 计算阈值 thresh = unionArea * iou_thresh
            "fmul s4, s3, %s[iou_thresh]\n"

            // 比较 interArea 与阈值
            "fcmpe s0, s4\n"
            "ble 3f\n"  // 如果 interArea <= thresh，跳过

            // 设置 pValidIndexes[n] = false
            "mov w16, #0\n"
            "strb w16, [%[pValidIndexes], x7]\n"

            "3:\n"
            "add x7, x7, #1\n"
            "cmp x7, x8\n"
            "blt 1b\n"

            "2:\n"
            :
            : [pData]"r"(pData), [pValidIndexes]"r"(pValidIndexes), [iou_thresh]"w"(iou_thresh),
              [m_reg]"r"(m), [n_reg]"r"(n), [size_reg]"r"(size)
            : "x6", "x7", "x8", "w9", "x10", "x11", "w16",
              "v0", "v1", "v2", "v3", "v4", "v5", "v6",
              "s0", "s1", "s2", "s3", "s4", "s5", "s6",
              "cc"
        );


    }
    // 清理内存
    delete[] pValidIndexes;
    delete[] pData;
}
