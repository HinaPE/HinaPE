#ifndef HINAPE_RAND_H
#define HINAPE_RAND_H

#include "../lib/mathlib.h"

namespace RNG
{

// Generate random float in the range [0,1)
auto unit() -> float;

// Generate random integer in the range [min,max)
auto integer(int min, int max) -> int;

// Return true with probability p and false with probability 1-p
auto coin_flip(float p = 0.5f) -> bool;

// Seed the current thread's PRNG
void seed();
} // namespace RNG

#endif
