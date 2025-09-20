#include "include/cfar_ca.h"
#include <algorithm>
#include <cmath>

Detections cfar_ca(const std::vector<float>& mag, int R, int D, const CFARConfig& cfg) {
    Detections det;
    const int g = cfg.guard, t = cfg.train;
    for (int r = t+g; r < R - (t+g); ++r) {
        for (int d = t+g; d < D - (t+g); ++d) {
            float sum = 0.0f; int count = 0;
            for (int dr = -t-g; dr <= t+g; ++dr) {
                for (int dd = -t-g; dd <= t+g; ++dd) {
                    if (std::abs(dr) <= g && std::abs(dd) <= g) continue; // guard region
                    sum += mag[(r+dr)*D + (d+dd)];
                    ++count;
                }
            }
            float noise = sum / std::max(1, count);
            float th = cfg.k * noise;
            float cut = mag[r*D + d];
            if (cut > th) det.points.emplace_back(r, d, cut);
        }
    }
    return det;
}
