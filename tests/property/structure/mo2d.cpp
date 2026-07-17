#include <cassert>
#include <random>
#include <vector>
#include "structure/mo.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(20260715);
    for (int repetition = 0; repetition < 200; ++repetition) {
        const int limit_x = random() % 100;
        const int limit_y = random() % 100;
        const int count = 1 + random() % 300;
        Mo2d mo(limit_x, limit_y);
        vector<pair<int, int>> queries(count);
        for (auto& [x, y] : queries) {
            x = random() % (limit_x + 1);
            y = random() % (limit_y + 1);
            mo.add(x, y);
        }
        int x = 0, y = 0;
        vector<long long> answers(count);
        mo.run(
            [&](int old) { assert(old == x); ++x; },
            [&](int old) { assert(old == x); --x; },
            [&](int old) { assert(old == y); ++y; },
            [&](int old) { assert(old == y); --y; },
            [&](int id) { answers[id] = 1LL * x * x + 3LL * y * y + x * y; }
        );
        for (int id = 0; id < count; ++id) {
            const auto [expected_x, expected_y] = queries[id];
            assert(answers[id] ==
                   1LL * expected_x * expected_x + 3LL * expected_y * expected_y +
                       expected_x * expected_y);
        }
    }
}
