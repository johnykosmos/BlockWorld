#pragma once


#include "utils/UtilTypedefs.hpp"



/**
 * @class LCG
 * @brief A simple Linear Congruential Generator (LCG) for pseudo-random number generation.
 * 
 * Provides functionality to generate pseudo-random floating-point numbers based on an internal 64-bit state.
 */
class LCG {
    private:
        /**
         * @brief Internal state of the random number generator.
         */
        u64 state;

        /**
         * @brief Generates the next pseudo-random 64-bit integer.
         * @return The next value in the sequence.
         */
        u64 next();

    public:
        /**
         * @brief Constructs an LCG with the given seed value.
         * @param seed The initial seed used to initialize the state.
         */
        LCG(uint seed);

        /**
         * @brief Generates a pseudo-random float in the range [0.0, 1.0).
         * @return A random float between 0.0 (inclusive) and 1.0 (exclusive).
         */
        float nextFloat();

        /**
         * @brief Generates a pseudo-random float in a specified range.
         * @param min The minimum value of the range (inclusive).
         * @param max The maximum value of the range (exclusive).
         * @return A random float between min and max.
         */
        float nextFloatInRange(float min, float max);
};

