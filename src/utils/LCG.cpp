#include "LCG.hpp"


LCG::LCG(uint seed) {
    state = seed;
}

u64 LCG::next() {
    state =  (25214903917u * state + 11) & ((1ULL << 48) - 1);
    return state;
}

float LCG::nextFloat() {
    return static_cast<float>(next()) / static_cast<float>(1ULL << 48);
}

float LCG::nextFloatInRange(float min, float max) {
    return min + nextFloat() * (max - min);
}
