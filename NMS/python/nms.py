import cv2
import time

def load_box(filepath):
    with open(filepath, "r") as f:
        lines = f.readlines()
    boxes = []
    scores = []
    for line in lines[1:]:
        data = line.strip().split("  ")
        box = [float(x) for x in data[:4]]
        box = [(box[0] + box[2]) / 2, (box[1] + box[3])/2, max(box[2] - box[0],0), max(box[3] - box[1],0)]
        score = float(data[5])
        boxes.append(box)
        scores.append(score)
    return boxes, scores


if __name__ == "__main__":
    filepath = "/data/haibo/develop/ai_cpp_tool/Data/bbox.txt"
    boxes, scores = load_box(filepath)
    confidence_threshold = 0.2
    nms_threshold = 0.4

    # 使用 OpenCV 的 NMS
    start_time = time.time()
    indices = cv2.dnn.NMSBoxes(boxes, scores, confidence_threshold, nms_threshold)
    end_time = time.time()
    elapsed_time = (end_time - start_time) * 1_000_000  # 转换为微秒
    print(f"经过的时间: {elapsed_time:.2f} 微秒")

    # 输出保留的边界框
    print(f"保留的边界框数量: {len(indices)}")
    # for i in indices:
    #     box = boxes[i[0]]
    #     print(f"保留的边界框: {box}")
