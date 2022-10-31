#include "rand.h"
#include "../lib/mathlib.h"

#include <ctime>
#include <random>
#include <thread>

namespace RNG
{

static thread_local std::mt19937 rng;

auto unit() -> float
{
    std::uniform_real_distribution<float> d(0.0f, 1.0f);
    return d(rng);
}

auto integer(int min, int max) -> int
{
    std::uniform_int_distribution<int> d(min, max - 1);
    return d(rng);
}

auto coin_flip(float p) -> bool
{
    return unit() < p;
}

void seed()
{
    std::random_device r;
    std::random_device::result_type seed = r() ^ (std::random_device::result_type) std::hash<std::thread::id>()(std::this_thread::get_id()) ^
                                           (std::random_device::result_type) std::hash<time_t>()(std::time(nullptr));
    rng.seed(seed);
}

} // namespace RNG
