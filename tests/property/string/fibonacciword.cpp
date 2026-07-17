#include <cassert>
#include <random>
#include <string>

#include "string/fibonacciword.hpp"

int main() {
    std::mt19937 random(20260715);
    for (int trial = 0; trial < 500; ++trial) {
        std::string first(1 + random() % 4, 'a');
        std::string second(1 + random() % 4, 'a');
        for (char& c : first) c = "abc"[random() % 3];
        for (char& c : second) c = "abc"[random() % 3];
        std::string previous = first, current = second;
        while (current.size() < 200) {
            std::string next = current + previous;
            previous = std::move(current);
            current = std::move(next);
        }
        poe::fibonacciword word(first, second, 200);
        for (int right = 0; right <= 200; ++right) {
            for (char c : std::string("abc")) {
                assert(word.count(c, right) ==
                       static_cast<unsigned long long>(std::count(current.begin(), current.begin() + right, c)));
            }
            if (right < 200) assert(word[right] == current[right]);
        }
    }
}
