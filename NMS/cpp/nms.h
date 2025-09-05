#include "ai_tools.h"

using std::vector;
// C++ NMS
struct gridCell{
    float score = 0.0;
    int count = 0;
    int idx;
};

class postBoxMap {
    private:
        std::vector<int> m_grid;
        int m_height, m_width;
        int m_width_count, m_height_count;
        std::vector<gridCell> m_gridCellVec;     
        int m_count = 0;
    public:
        postBoxMap(int height, int width, std::vector<int> grid = {4,4}) {
            m_height = height;
            m_width = width;
            m_grid = grid;

            m_width_count = (m_width - 1) / m_grid[0] + 1;
            m_height_count = (m_height - 1) / m_grid[1] + 1;
            m_gridCellVec.resize(m_width_count * m_height_count, {0.0f, 0, 0});
        }
        void filter(const vector<Box>& dets, std::vector<uint8_t>& invalid) {
            if (dets.size() == 0) return;
            for (int i = 0; i < dets.size(); i++) {
                int x_grid = static_cast<int>(dets[i].x / m_grid[0]);
                int y_grid = static_cast<int>(dets[i].y / m_grid[1]);
                int idx = y_grid * m_width_count + x_grid;
                if(m_gridCellVec[idx].count != m_count) {
                    m_gridCellVec[idx].count = m_count;
                    m_gridCellVec[idx].score = 0.0f;
                }
                if(std::abs(m_gridCellVec[idx].score - 0.0f) < 0.001) {
                    m_gridCellVec[idx].score = dets[i].score;
                    m_gridCellVec[idx].idx = i;
                } else if (dets[i].score > m_gridCellVec[idx].score) {
                    m_gridCellVec[idx].score = dets[i].score;
                    invalid[m_gridCellVec[idx].idx] = 1;
                    m_gridCellVec[idx].idx = i;
                } else {
                    invalid[i] = 1;
                }
            }
            m_count ++;
        }



};
// C++ NMS
void NMS(vector<Box>& dets, vector<Box>& res, float conf, postBoxMap& postFilter);