#include "math/nimber.hpp"

using namespace std;
using namespace poe;

int main() {
    constexpr int limit = 32;
    vector<vector<int>> brute(limit, vector<int>(limit));
    for (int left = 0; left < limit; ++left) {
        for (int right = 0; right < limit; ++right) {
            vector<bool> used(2048);
            for (int smaller_left = 0; smaller_left < left; ++smaller_left) {
                for (int smaller_right = 0; smaller_right < right; ++smaller_right) {
                    const int value = brute[smaller_left][right] ^
                                      brute[left][smaller_right] ^
                                      brute[smaller_left][smaller_right];
                    assert(value < static_cast<int>(used.size()));
                    used[value] = true;
                }
            }
            while (used[brute[left][right]]) ++brute[left][right];
            assert(nim_product(left, right) ==
                   static_cast<uint64_t>(brute[left][right]));
        }
    }

    mt19937_64 random(274);
    for (int iteration = 0; iteration < 10000; ++iteration) {
        const uint64_t a = random();
        const uint64_t b = random();
        const uint64_t c = random();
        assert(nim_product(a, b) == nim_product(b, a));
        assert(nim_product(a, b ^ c) ==
               (nim_product(a, b) ^ nim_product(a, c)));
        assert(nim_product(a, 1) == a && nim_product(a, 0) == 0);
    }
}
