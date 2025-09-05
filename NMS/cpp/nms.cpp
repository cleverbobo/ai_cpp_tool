#include <algorithm>

#include "nms.h"

void NMS(vector<Box>& dets, vector<Box>& res, float conf, postBoxMap& postFilter) {
    if (dets.size() == 0) return;
    std::sort(dets.begin(), dets.end(),[](const Box& a, const Box& b){return a.score > b.score;});

    std::vector<uint8_t> invalid(dets.size(), 0);
    postFilter.filter(dets, invalid);

    std::vector<float> pData(5 * dets.size());

    for (int i = 0; i < dets.size(); i++) {
        if (invalid[i]) continue;
        pData[5*i + 0] = dets[i].x - dets[i].width / 2;
        pData[5*i + 1] = dets[i].y - dets[i].height / 2;
        pData[5*i + 2] = dets[i].x + dets[i].width / 2;
        pData[5*i + 3] = dets[i].y + dets[i].height / 2;
        pData[5*i + 4] = dets[i].width * dets[i].height;
    }

    for (int i =0; i < dets.size(); i++) {
        if (invalid[i]) continue;
        res.push_back(dets[i]);
        auto left = &pData[5*i];

        
        for (int j = i + 1; j < dets.size(); j++) {
            if( invalid[j]) continue;
            auto right = &pData[5*j];

            const float inter_x1 = std::max(*(left+0), *(right+0));
            const float inter_y1 = std::max(*(left+1), *(right+1));
            const float inter_x2 = std::min(*(left+2), *(right+2));
            const float inter_y2 = std::min(*(left+3), *(right+3));

            if (inter_x2 <= inter_x1 || inter_y2 <= inter_y1){
                continue;
            }
            auto inter_area  = (inter_y2 -inter_y1) * (inter_x2 - inter_x1);
            invalid[j] =  inter_area  > (*(left+4) + *(right+4) - inter_area) * conf; 
        }
    }
}