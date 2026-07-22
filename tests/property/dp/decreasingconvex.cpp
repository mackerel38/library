#include "dp/decreasingconvex.hpp"

using namespace std;
using namespace poe;

int main() {
    constexpr int limit = 25;
    mt19937 random(275);
    const auto next = [&](int bound) {
        return static_cast<int>(random() % static_cast<unsigned int>(bound));
    };
    for (int iteration = 0; iteration < 5000; ++iteration) {
        decreasingconvexsequence<long long> function;
        vector<long long> brute(limit + 1);
        for (int step = 0; step < 20; ++step) {
            if (next(2) == 0) {
                const int required = next(16);
                const int cost = next(11);
                function.merge(decreasingconvexsequence<long long>(required, cost));
                for (int x = 0; x <= limit; ++x) {
                    brute[x] += static_cast<long long>(max(required - x, 0)) * cost;
                }
            } else {
                const int required = next(16);
                const int cost = next(11);
                function.suffix_min(cost, required);
                vector<long long> next_brute(limit + 1);
                for (int x = 0; x <= limit; ++x) {
                    next_brute[x] = numeric_limits<long long>::max();
                    for (int y = max(x, required); y <= limit; ++y) {
                        next_brute[x] = min(next_brute[x],
                            static_cast<long long>(y - x) * cost + brute[y]);
                    }
                }
                brute = std::move(next_brute);
            }
            for (int x = 0; x <= limit; ++x) assert(function[x] == brute[x]);
            assert(function.zero() == brute[0]);
        }
    }
}
