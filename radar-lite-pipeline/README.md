# Radar-Lite Signal Pipeline (PC-only)

A hardware-free **C++17** project demonstrating a simplified radar signal chain:
**FMCW chirp generation → FFT → CFAR (Cell-Averaging) → Kalman tracking**, with a CLI runner and Python visualization.

## Features
- Synthetic FMCW data generator with configurable targets (range, velocity, SNR).
- 1D/2D FFT (naive radix-2 implementation included; no external deps).
- CA-CFAR detector (tunable guard/train settings).
- Constant-velocity Kalman filter tracker.
- CLI saves detections to `out/detections.csv` and heatmaps to `out/rd_map.csv`.
- Unit tests (GoogleTest via CMake `FetchContent`) + GitHub Actions CI.
- Cross-platform: Linux/macOS/Windows (MinGW).

## Build
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j
./radar-lite --frames 20 --targets 3 --snr 12
```

CSV files will be written under `out/`. Use the Python script to visualize:
```bash
python3 python/visualize.py out/rd_map.csv out/detections.csv
```

## CLI
```
./radar-lite [--frames N] [--targets K] [--snr dB] [--range_bins N] [--doppler_bins M]
```

## Folder layout
```
radar-lite-pipeline/
├─ README.md
├─ docs/
│  ├─ architecture.md
│  └─ algorithms.md
├─ cpp/
│  ├─ include/
│  │  ├─ types.h
│  │  ├─ chirp_gen.h
│  │  ├─ fft_wrapper.h
│  │  ├─ cfar_ca.h
│  │  └─ tracker_kf.h
│  ├─ chirp_gen.cpp
│  ├─ fft_wrapper.cpp
│  ├─ cfar_ca.cpp
│  ├─ tracker_kf.cpp
│  ├─ main.cpp
│  └─ CMakeLists.txt
├─ tests/
│  ├─ CMakeLists.txt
│  ├─ test_cfar.cpp
│  └─ test_tracker.cpp
├─ python/
│  └─ visualize.py
├─ CMakeLists.txt
└─ .github/workflows/ci.yml
```

## Notes
This is an educational, CPU-only pipeline intended for learning. Numerics and models are simplified.
