#include "geometry/latticemove.hpp"

using namespace std;
using namespace poe;

optional<long long> brute_pair(
    point<long long> target,
    point<long long> first,
    point<long long> second
) {
    optional<long long> answer;
    for (long long x = 0; x <= 40; ++x) {
        for (long long y = 0; y <= 40; ++y) {
            if (first * x + second * y == target &&
                (!answer || x + y < *answer)) answer = x + y;
        }
    }
    return answer;
}

int main() {
    vector<point<long long>> vectors;
    for (long long x = -2; x <= 2; ++x) {
        for (long long y = -2; y <= 2; ++y) vectors.push_back({x, y});
    }
    for (const auto first : vectors) {
        for (const auto second : vectors) {
            for (long long x = -8; x <= 8; ++x) {
                for (long long y = -8; y <= 8; ++y) {
                    const point<long long> target{x, y};
                    const auto expected = brute_pair(target, first, second);
                    const auto actual = minimum_lattice_moves(target, first, second);
                    assert(actual == expected);
                }
            }
        }
    }

    mt19937 random(271);
    for (int iteration = 0; iteration < 10000; ++iteration) {
        vector<point<long long>> steps(1 + random() % 8);
        for (auto& step : steps) step = vectors[random() % vectors.size()];
        const point<long long> target{
            static_cast<int>(random() % 17) - 8,
            static_cast<int>(random() % 17) - 8,
        };
        optional<long long> expected;
        for (size_t first = 0; first < steps.size(); ++first) {
            for (size_t second = first; second < steps.size(); ++second) {
                const auto candidate = brute_pair(target, steps[first], steps[second]);
                if (candidate && (!expected || *candidate < *expected)) expected = candidate;
            }
        }
        assert(minimum_two_kind_lattice_moves(target, steps) == expected);
    }
}
