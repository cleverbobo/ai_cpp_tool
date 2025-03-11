#include "ai_tools.h"

// C++ NMS
void NMS_Cpp(BoxVec &dets, BoxVec& res, float nmsConfidence=0.2, float iou_thresh=0.4);

// 内敛汇编NMS
void NMS_Compile(BoxVec &dets, BoxVec& res,float nmsConfidence=0.2, float iou_thresh=0.4);

void NMS_Cpp_Optimized(const BoxVec &dets, BoxVec &res, float nmsConfidence=0.2, float iou_thresh=0.4);

void NMS_Cpp_Optimized_plus(const BoxVec &dets, BoxVec &res, float nmsConfidence=0.2, float iou_thresh=0.4);