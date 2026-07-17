#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <random>
#include <vector>

#include "structure/median.hpp"

int main() {
    std::mt19937 random(20260715);
    poe::median<long long> values;
    std::vector<long long> brute;
    for (int operation = 0; operation < 5000; ++operation) {
        if (brute.empty() || random() % 3 != 0) {
            const long long value = static_cast<int>(random() % 101) - 50;
            values.add(value);
            brute.push_back(value);
        } else {
            const int index = random() % brute.size();
            values.erase(brute[index]);
            brute.erase(brute.begin() + index);
        }
        std::sort(brute.begin(), brute.end());
        assert(values.size() == static_cast<int>(brute.size()));
        if (brute.empty()) {
            assert(values.empty() && values.deviation() == 0);
            continue;
        }
        assert(values.lower() == brute[(brute.size() - 1) / 2]);
        assert(values.upper() == brute[brute.size() / 2]);
        long long deviation = 0;
        for (const long long value : brute) deviation += std::abs(value - values.lower());
        assert(values.deviation() == deviation);
    }
}
