#include "string/rotation.hpp"

int main() {
    std::mt19937 random(193847);
    for (int n = 0; n <= 60; ++n) {
        for (int trial = 0; trial < 1000; ++trial) {
            std::vector<int> values(n);
            for (int& value : values) value = random() % 5;
            std::vector<std::vector<int>> rotations;
            for (int start = 0; start < std::max(1, n); ++start) {
                rotations.push_back(poe::rotated(values, start));
            }
            const auto minimum = *std::min_element(rotations.begin(), rotations.end());
            const auto maximum = *std::max_element(rotations.begin(), rotations.end());
            const int minimum_index = poe::minimum_rotation(values);
            const int maximum_index = poe::maximum_rotation(values);
            assert(poe::rotated(values, minimum_index) == minimum);
            assert(poe::rotated(values, maximum_index) == maximum);
            for (int start = 0; start < n; ++start) {
                if (rotations[start] == minimum) {
                    assert(minimum_index <= start);
                }
                if (rotations[start] == maximum) {
                    assert(maximum_index <= start);
                }
            }
            for (int start = 0; start < std::max(1, n); ++start) {
                const auto target = poe::rotated(values, start);
                int expected = 0;
                while (expected < n && poe::rotated(values, expected) != target) ++expected;
                assert(poe::rotation_distance(values, target) == expected);
            }
            auto impossible = values;
            impossible.push_back(100);
            if (!values.empty()) impossible.erase(impossible.begin());
            if (!values.empty() && std::ranges::find(rotations, impossible) == rotations.end()) {
                assert(poe::rotation_distance(values, impossible) == -1);
            }
        }
    }
}
