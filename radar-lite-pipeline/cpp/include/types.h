#pragma once
#include <vector>
#include <complex>
#include <cstdint>
#include <cmath>
#include <string>
#include <tuple>

using cfloat = std::complex<float>;

struct Target {
    float range_m;
    float vel_mps;
    float snr_db;
};

struct Detections {
    // (range_bin, doppler_bin, magnitude)
    std::vector<std::tuple<int,int,float>> points;
};

struct Track {
    float range_m;
    float vel_mps;
    int id;
};
