#pragma once

#include <random>

using namespace std;

class Utilities {
public:
    // Randomly generate a number
    static int NumberGenerator(int min_val, int max_val) {

        // 1. Create a random_device to seed the generator
        //    random_device provides non-deterministic random numbers (from hardware or OS)
        std::random_device rd;

        // 2. Create a Mersenne Twister engine (a common and good pseudo-random number generator)
        //    Seed it with the random_device for better randomness
        std::mt19937 gen(rd());

        // 3. Create a uniform_int_distribution for the desired range
        //    This ensures numbers are distributed evenly within the range [min_val, max_val]
        std::uniform_int_distribution<> distrib(min_val, max_val);

        // 4. Generate the random number
        int random_number = distrib(gen);

        return random_number;
    }
};