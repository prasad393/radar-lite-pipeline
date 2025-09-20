#pragma once
#include "types.h"

// Simple 1D range + velocity Kalman (constant-velocity model).
// State x = [range; vel], measurement z = range.
struct KF {
    float r_var = 1.0f;   // meas noise var
    float q_var = 0.1f;   // process noise var
    float dt = 0.05f;

    // state
    float r=0.0f, v=0.0f;
    float P11=1, P12=0, P21=0, P22=1;

    void predict() {
        // x = F x, P = F P F^T + Q
        float r2 = r + dt * v;
        float v2 = v;
        float F11=1, F12=dt, F21=0, F22=1;
        float P11n = F11*P11 + F12*P21;
        float P12n = F11*P12 + F12*P22;
        float P21n = F21*P11 + F22*P21;
        float P22n = F21*P12 + F22*P22;
        // FPF^T
        float P11p = P11n*F11 + P12n*F12 + q_var;
        float P12p = P11n*F21 + P12n*F22;
        float P21p = P21n*F11 + P22n*F12;
        float P22p = P21n*F21 + P22n*F22 + q_var;
        r=r2; v=v2; P11=P11p; P12=P12p; P21=P21p; P22=P22p;
    }

    void update(float z) {
        // y = z - Hx ; H = [1 0]
        float y = z - r;
        float S = P11 + r_var;
        float K1 = P11 / S;
        float K2 = P21 / S;
        r = r + K1*y;
        v = v + K2*y;
        // P = (I-KH)P
        float P11n = (1-K1)*P11;
        float P12n = (1-K1)*P12;
        float P21n = -K2*P11 + P21;
        float P22n = -K2*P12 + P22;
        P11=P11n; P12=P12n; P21=P21n; P22=P22n;
    }
};
