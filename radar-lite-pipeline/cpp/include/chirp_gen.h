#pragma once
#include "types.h"

struct RadarConfig {
    int num_chirps = 32;
    int samples_per_chirp = 256;
    float fs = 1.0e6f;         // sample rate [Hz]
    float slope = 20.0e12f;    // Hz/s (chirp slope)
    float fc = 77.0e9f;        // carrier [Hz]
    float c = 3.0e8f;          // speed of light [m/s]
};

// Generate complex baseband beat signal for K targets over N chirps x M samples.
std::vector<cfloat> generate_chirps(const RadarConfig& cfg,
                                    const std::vector<Target>& tgts);
