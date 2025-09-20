#pragma once
#include "types.h"

// In-place radix-2 FFT (N must be power of two). inverse=false for FFT, true for IFFT.
void fft(std::vector<cfloat>& a, bool inverse=false);

// Convenience wrappers for 2D FFT on row-major [rows x cols] array.
void fft2d(std::vector<cfloat>& data, int rows, int cols, bool inverse=false);
