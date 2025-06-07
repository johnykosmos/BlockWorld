#pragma once


#include "utils/Typedefs.hpp"


class LCG {
    private:
        u64 state;

    private:
        u64 next();

    public:
        LCG(uint seed);
        float nextFloat();
        float nextFloatInRange(float min, float max);
};
