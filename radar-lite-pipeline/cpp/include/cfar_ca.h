#pragma once
#include "types.h"

struct CFARConfig {
    int guard = 2;
    int train = 8;
    float k = 3.0f; // scaling factor
};

// Input: magnitude (power) map of size [R x D]; returns detections (bin indices).
Detections cfar_ca(const std::vector<float>& mag, int R, int D, const CFARConfig& cfg);
