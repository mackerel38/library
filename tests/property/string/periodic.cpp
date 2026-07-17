#include <cassert>
#include <random>
#include <set>
#include <string>
#include <vector>

#include "string/periodic.hpp"

int main() {
    std::mt19937 random(20260716);
    for (int trial = 0; trial < 1000; ++trial) {
        const int count = 1 + random() % 30;
        std::vector<std::string> strings(count);
        for (std::string& text : strings) {
            const int length = 1 + random() % 7;
            text.resize(length);
            for (char& character : text) character = "ab"[random() % 2];
        }
        std::set<std::string> used;
        std::vector<long long> brute;
        for (const std::string& text : strings) {
            std::string repeated;
            long long count = 0;
            do {
                repeated += text;
                ++count;
            } while (used.contains(repeated));
            used.insert(repeated);
            brute.push_back(count);
        }
        assert(poe::minimum_unique_repetition_counts(strings) == brute);
    }
}
