#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <tuple>

#include "include/types.h"
#include "include/chirp_gen.h"
#include "include/fft_wrapper.h"
#include "include/cfar_ca.h"
#include "include/tracker_kf.h"

static void write_csv(const std::string& path, const std::vector<std::vector<float>>& mat) {
    std::ofstream f(path);
    for (auto& row : mat) {
        for (size_t i=0;i<row.size();++i) {
            f << row[i] << (i+1<row.size() ? "," : "");
        }
        f << "\n";
    }
}

static void write_det(const std::string& path, const Detections& det) {
    std::ofstream f(path);
    f << "range_bin,doppler_bin,magnitude\n";
    for (auto& p : det.points) {
        f << std::get<0>(p) << "," << std::get<1>(p) << "," << std::get<2>(p) << "\n";
    }
}

int main(int argc, char** argv) {
    // Defaults
    int frames = 20, targets = 3, Rbins = 256, Dbins = 32;
    float snr = 12.0f;

    for (int i=1;i<argc;++i) {
        std::string a = argv[i];
        auto getv = [&](int& out){ if (i+1<argc){ out=std::stoi(argv[++i]); } };
        auto getf = [&](float& out){ if (i+1<argc){ out=std::stof(argv[++i]); } };
        if (a=="--frames") getv(frames);
        else if (a=="--targets") getv(targets);
        else if (a=="--snr") getf(snr);
        else if (a=="--range_bins") getv(Rbins);
        else if (a=="--doppler_bins") getv(Dbins);
    }

    std::filesystem::create_directories("out");

    RadarConfig cfg;
    cfg.num_chirps = Dbins;
    cfg.samples_per_chirp = Rbins;

    // Create synthetic targets
    std::vector<Target> tg;
    for (int k=0;k<targets;++k) {
        Target t;
        t.range_m = 10.0f + 15.0f * k; // spaced
        t.vel_mps = -2.0f + k;         // varying velocities
        t.snr_db = snr;
        tg.push_back(t);
    }

    // Accumulate RD map over frames (simple average)
    std::vector<std::vector<float>> rd_avg(Dbins, std::vector<float>(Rbins, 0.0f));
    CFARConfig cfar;
    Detections det_all;

    for (int fidx=0; fidx<frames; ++fidx) {
        auto bb = generate_chirps(cfg, tg); // Dbins x Rbins
        // Range FFT for each chirp
        for (int d=0; d<Dbins; ++d) {
            std::vector<cfloat> row(Rbins);
            for (int r=0; r<Rbins; ++r) row[r] = bb[d*Rbins + r];
            fft(row, false);
            for (int r=0; r<Rbins; ++r) bb[d*Rbins + r] = row[r];
        }
        // Doppler FFT across chirps per range bin
        for (int r=0; r<Rbins; ++r) {
            std::vector<cfloat> col(Dbins);
            for (int d=0; d<Dbins; ++d) col[d] = bb[d*Rbins + r];
            fft(col, false);
            for (int d=0; d<Dbins; ++d) bb[d*Rbins + r] = col[d];
        }
        // Magnitude map (Dbins x Rbins)
        std::vector<float> mag(Dbins*Rbins);
        for (int d=0; d<Dbins; ++d) {
            for (int r=0; r<Rbins; ++r) {
                auto v = bb[d*Rbins + r];
                mag[d*Rbins + r] = std::norm(v);
                rd_avg[d][r] += mag[d*Rbins + r] / frames;
            }
        }
        // CFAR on this frame
        auto det = cfar_ca(mag, Dbins, Rbins, cfar);
        det_all.points.insert(det_all.points.end(), det.points.begin(), det.points.end());
    }

    // Write outputs
    write_csv("out/rd_map.csv", rd_avg);
    write_det("out/detections.csv", det_all);

    // Simple tracking demo: run KF on strongest detection per frame (range only)
    KF kf;
    kf.r = tg[0].range_m; // init near first target
    kf.v = tg[0].vel_mps;
    for (int step=0; step<frames; ++step) {
        kf.predict();
        // crude measurement from averaged map: argmax over range for mid doppler
        int d = Dbins/2;
        int argmax = 0; float best=0.0f;
        for (int r=0;r<Rbins;++r){
            float v = rd_avg[d][r];
            if (v>best){best=v; argmax=r;}
        }
        // map bin to meters (toy scaling): assume max range 128 m
        float meas_range = (float)argmax / (Rbins-1) * 128.0f;
        kf.update(meas_range);
    }

    std::cout << "Wrote: out/rd_map.csv and out/detections.csv\n";
    std::cout << "Final KF range=" << kf.r << " m, vel=" << kf.v << " m/s\n";
    return 0;
}
