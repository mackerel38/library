#include <cassert>
#include <random>

#include "math/xorbasis.hpp"

int main() {
    std::mt19937 random(20260723);
    for (int test = 0; test < 3000; ++test) {
        const int count = 1 + random() % 15;
        const unsigned upper = random() % 32;
        std::vector<std::pair<unsigned, unsigned>> paired(count);
        for (auto& [first, second] : paired) {
            first = random() % 32;
            second = random() % 32;
        }
        std::optional<unsigned> expected;
        for (int mask = 1; mask < (1 << count); ++mask) {
            unsigned first = 0, second = 0;
            for (int index = 0; index < count; ++index) {
                if (mask >> index & 1) {
                    first ^= paired[index].first;
                    second ^= paired[index].second;
                }
            }
            if (first <= upper && (!expected || *expected < second)) expected = second;
        }
        assert((poe::maximum_paired_xor_under<unsigned, 5>(paired, upper) == expected));
    }
    for (int test = 0; test < 500; ++test) {
        const int count = random() % 12;
        std::vector<unsigned long long> values(count);
        poe::xorbasis<> basis;
        for (auto& value : values) value = random() % 1024, basis.add(value);
        std::vector<unsigned long long> brute{0};
        for (const auto value : values) {
            const int size = static_cast<int>(brute.size());
            for (int i = 0; i < size; ++i) brute.push_back(brute[i] ^ value);
        }
        std::ranges::sort(brute);
        brute.erase(std::unique(brute.begin(), brute.end()), brute.end());
        assert(static_cast<unsigned long long>(brute.size()) == (1ULL << basis.rank()));
        for (int value = 0; value < 1024; ++value) {
            assert(basis.contains(value) == std::binary_search(brute.begin(), brute.end(), value));
        }
        assert(basis.min() == brute.front() && basis.max() == brute.back());
        for (int i = 0; i < static_cast<int>(brute.size()); ++i) assert(basis.kth(i) == brute[i]);
    }
}
