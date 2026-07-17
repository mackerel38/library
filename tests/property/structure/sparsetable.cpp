#include "structure/sparsetable.hpp"

int minimum(int left, int right) {
    return std::min(left, right);
}

std::string concatenate(const std::string& left, const std::string& right) {
    return left + right;
}

int main() {
    std::mt19937 random(99173);
    for (int n = 1; n <= 100; ++n) {
        std::vector<int> values(n);
        for (int& value : values) value = static_cast<int>(random() % 1000);
        poe::sparsetable<int, minimum> table(values);
        for (int left = 0; left < n; ++left) {
            int expected = values[left];
            for (int right = left + 1; right <= n; ++right) {
                expected = std::min(expected, values[right - 1]);
                assert(table.prod(left, right) == expected);
            }
        }

        std::vector<std::string> letters(n);
        for (auto& value : letters) value = std::string(1, 'a' + random() % 26);
        poe::disjointsparsetable<std::string, concatenate> disjoint(letters);
        for (int left = 0; left < n; ++left) {
            std::string expected;
            for (int right = left + 1; right <= n; ++right) {
                expected += letters[right - 1];
                assert(disjoint.prod(left, right) == expected);
            }
        }
    }
}
