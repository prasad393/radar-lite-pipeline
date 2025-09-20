#include <gtest/gtest.h>
#include "cpp/include/cfar_ca.h"

TEST(CFAR, BasicThresholding) {
    int R=32, D=16;
    std::vector<float> mag(R*D, 1.0f); // noise floor 1
    // inject a strong target
    mag[10*D + 8] = 20.0f;
    CFARConfig cfg; cfg.guard=1; cfg.train=4; cfg.k=3.0f;
    auto det = cfar_ca(mag, R, D, cfg);
    bool found=false;
    for (auto& p: det.points) {
        if (std::get<0>(p)==10 && std::get<1>(p)==8) found=true;
    }
    EXPECT_TRUE(found);
}
