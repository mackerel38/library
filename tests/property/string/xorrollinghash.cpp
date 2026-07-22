#include "string/xorrollinghash.hpp"

using namespace std;
using namespace poe;

uint64_t brute_hash(const vector<uint64_t>& values, int left, int right,
                    uint64_t base) {
    uint64_t result = 0;
    for (int index = left; index < right; ++index) {
        result = nim_product(result, base) ^ values[index];
    }
    return result;
}

int main() {
    mt19937_64 random(20260718);
    constexpr uint64_t base = 0x123456789abcdef0ULL;
    for (int iteration = 0; iteration < 1000; ++iteration) {
        const int n = 1 + static_cast<int>(random() % 50);
        vector<uint64_t> values(n);
        for (auto& value : values) value = random();
        xorrollinghash hash(values, base);
        for (int query = 0; query < 100; ++query) {
            int left = static_cast<int>(random() % (n + 1));
            int right = static_cast<int>(random() % (n + 1));
            if (left > right) swap(left, right);
            assert(hash.hash(left, right) == brute_hash(values, left, right, base));

            const int length = static_cast<int>(random() % (n + 1));
            const int first = static_cast<int>(random() % (n - length + 1));
            const int second = static_cast<int>(random() % (n - length + 1));
            const int target = static_cast<int>(random() % (n + 1));
            const int target_right = target +
                static_cast<int>(random() % (n - target + 1));
            int expected = 0;
            while (expected < length && target + expected < target_right &&
                   (values[first + expected] ^ values[second + expected]) ==
                       values[target + expected]) {
                ++expected;
            }
            assert(hash.xor_lcp(first, second, length, target, target_right) == expected);
        }
    }
}
