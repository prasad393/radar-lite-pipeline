#include "include/fft_wrapper.h"
#include <cassert>

static void bit_reverse(std::vector<cfloat>& a) {
    size_t n = a.size();
    size_t j = 0;
    for (size_t i = 1; i < n; ++i) {
        size_t bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) std::swap(a[i], a[j]);
    }
}

void fft(std::vector<cfloat>& a, bool inverse) {
    size_t n = a.size();
    assert((n & (n - 1)) == 0 && "FFT size must be power of two");
    bit_reverse(a);
    for (size_t len = 2; len <= n; len <<= 1) {
        float ang = 2.0f * float(M_PI) / float(len) * (inverse ? -1.0f : 1.0f);
        cfloat wlen(std::cos(ang), std::sin(ang));
        for (size_t i = 0; i < n; i += len) {
            cfloat w(1.0f, 0.0f);
            for (size_t j = 0; j < len / 2; ++j) {
                cfloat u = a[i+j];
                cfloat v = a[i+j+len/2] * w;
                a[i+j] = u + v;
                a[i+j+len/2] = u - v;
                w *= wlen;
            }
        }
    }
    if (inverse) {
        for (auto& x : a) x /= float(n);
    }
}

void fft2d(std::vector<cfloat>& data, int rows, int cols, bool inverse) {
    // Row FFTs
    for (int r = 0; r < rows; ++r) {
        std::vector<cfloat> row(cols);
        for (int c = 0; c < cols; ++c) row[c] = data[r*cols + c];
        fft(row, inverse);
        for (int c = 0; c < cols; ++c) data[r*cols + c] = row[c];
    }
    // Column FFTs
    for (int c = 0; c < cols; ++c) {
        std::vector<cfloat> col(rows);
        for (int r = 0; r < rows; ++r) col[r] = data[r*cols + c];
        fft(col, inverse);
        for (int r = 0; r < rows; ++r) data[r*cols + c] = col[r];
    }
}
