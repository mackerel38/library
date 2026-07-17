#include <cassert>
#include <random>

#include "math/xorbasis.hpp"

int main() {
    std::mt19937 random(20260723);
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
