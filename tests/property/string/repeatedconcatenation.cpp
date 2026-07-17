#include <cassert>
#include <functional>
#include <random>
#include <string>
#include <vector>

#include "string/concatenation.hpp"

int main() {
    std::mt19937 random(20260716);
    for (int n = 1; n <= 5; ++n) {
        for (int trial = 0; trial < 30; ++trial) {
            std::vector<std::string> strings(n);
            for (auto& value : strings) {
                const int length = 1 + random() % 4;
                while (static_cast<int>(value.size()) < length) {
                    value += char('a' + random() % 3);
                }
            }
            const int count = random() % 6;
            std::string expected;
            bool first = true;
            std::function<void(int, std::string)> search =
                [&](int used, std::string value) {
                    if (used == count) {
                        if (first || value < expected) expected = std::move(value);
                        first = false;
                        return;
                    }
                    for (const auto& add : strings) search(used + 1, value + add);
                };
            search(0, {});
            assert(poe::minimum_repeated_concatenation(strings, count) == expected);
        }
    }
}
