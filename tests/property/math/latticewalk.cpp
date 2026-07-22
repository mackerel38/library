#include <bits/stdc++.h>
#include "math/latticewalk.hpp"

using namespace std;
using namespace poe;

int main() {
    constexpr int limit = 9;
    axiswalkcounter<998244353> counter(limit);
    for (int dimension = 1; dimension <= 3; ++dimension) {
        map<vector<int>, modint998244353> current, next;
        current[vector<int>(dimension)] = 1;
        for (int steps = 0; steps <= limit; ++steps) {
            for (const auto& [position, expected] : current) {
                modint998244353 actual;
                if (dimension == 1) actual = counter.count(steps, position[0]);
                if (dimension == 2) actual = counter.count(steps, position[0], position[1]);
                if (dimension == 3) {
                    actual = counter.count(steps, position[0], position[1], position[2]);
                }
                assert(actual == expected);
            }
            if (steps == limit) break;
            next.clear();
            for (const auto& [position, ways] : current) {
                for (int axis = 0; axis < dimension; ++axis) {
                    for (const int direction : {-1, 1}) {
                        auto moved = position;
                        moved[axis] += direction;
                        next[moved] += ways;
                    }
                }
            }
            current.swap(next);
        }
    }
}
