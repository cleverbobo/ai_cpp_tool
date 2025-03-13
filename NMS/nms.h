#include "ai_tools.h"

// C++ NMS
void NMS(const BoxVec &dets, BoxVec& res, float nmsConfidence=0.2, float iou_thresh=0.4);