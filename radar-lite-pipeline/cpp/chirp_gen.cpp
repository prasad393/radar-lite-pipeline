#include "include/chirp_gen.h"
#include <random>

static inline float db2lin(float db) { return std::pow(10.0f, db/20.0f); }

std::vector<cfloat> generate_chirps(const RadarConfig& cfg,
                                    const std::vector<Target>& tgts)
{
    const int N = cfg.num_chirps;
    const int M = cfg.samples_per_chirp;
    std::vector<cfloat> out;
    out.resize(N * M);

    std::mt19937 rng(42);
    std::normal_distribution<float> noise(0.0f, 0.02f);

    for (int n = 0; n < N; ++n) {
        for (int m = 0; m < M; ++m) {
            float t = m / cfg.fs;
            cfloat s(0.0f, 0.0f);
            for (auto& tg : tgts) {
                // Beat frequency approx: fb = 2*slope*R/c + 2*fc*V/c
                float fb = 2.0f * cfg.slope * tg.range_m / cfg.c
                         + 2.0f * cfg.fc * tg.vel_mps / cfg.c;
                float phase = 2.0f * float(M_PI) * (fb * t + n * (2.0f*cfg.fc*tg.vel_mps/cfg.c));
                float A = db2lin(tg.snr_db);
                s += A * cfloat(std::cos(phase), std::sin(phase));
            }
            s += cfloat(noise(rng), noise(rng));
            out[n*M + m] = s;
        }
    }
    return out;
}
