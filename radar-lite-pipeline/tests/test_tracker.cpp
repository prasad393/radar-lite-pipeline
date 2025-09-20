#include <gtest/gtest.h>
#include "cpp/include/tracker_kf.h"

TEST(KF, Converges) {
    KF kf; kf.r=0; kf.v=0; kf.dt=0.1f; kf.r_var=0.5f; kf.q_var=0.01f;
    // true range moves from 10m with vel 1 m/s
    float r=10.0f, v=1.0f;
    for (int i=0;i<100;++i) {
        kf.predict();
        r += v*kf.dt;
        float z = r + 0.1f; // constant small bias
        kf.update(z);
    }
    EXPECT_NEAR(kf.v, 1.0f, 0.2f);
    EXPECT_NEAR(kf.r, r, 0.5f);
}
