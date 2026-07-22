#include <cassert>
#include <random>

#include "recipe/thresholdsequence.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937_64 random(427);
    for (int test = 0; test < 300; ++test) {
        const long long up = random() % 10 + 1;
        const long long down = random() % 10 + 1;
        thresholdsequence<long long> sequence(up, down);
        vector<long long> thresholds;
        for (int length = 0; length < 70; ++length) {
            for (long long initial = -50; initial <= 50; ++initial) {
                long long expected = initial;
                for (const long long threshold : thresholds) {
                    expected += expected <= threshold ? up : -down;
                }
                assert(sequence.apply(initial) == expected);
            }
            const long long threshold = static_cast<long long>(random() % 101) - 50;
            thresholds.push_back(threshold);
            sequence.append(threshold);
        }
    }
}
