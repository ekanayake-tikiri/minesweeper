#pragma once
#include <iostream>
#include <random>
/*copyright: professor fox */

class Random {
    static std::mt19937 random;
public:
    static int Int(int min, int max);
};