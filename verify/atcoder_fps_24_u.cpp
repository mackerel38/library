#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_u"
#include <bits/stdc++.h>
#include "fps/holonomic.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    struct term { mint coefficient; int n_power, t_power, distance; };
    const vector<term> terms = {
        {144,0,0,1},{243,1,0,1},{3,2,0,1},{-408,0,1,1},{-432,1,1,1},{-10,2,1,1},{378,0,2,1},{243,1,2,1},{3,2,2,1},{-140,0,3,1},{-42,1,3,1},{18,0,4,1},
        {56,0,0,2},{270,1,0,2},{-26,2,0,2},{50,0,1,2},{-430,1,1,2},{28,2,1,2},{-250,0,2,2},{202,1,2,2},{-6,2,2,2},{166,0,3,2},{-30,1,3,2},{-30,0,4,2},
        {-800,0,0,3},{-138,1,0,3},{-10,2,0,3},{1662,0,1,3},{254,1,1,3},{20,2,1,3},{-1138,0,2,3},{-158,1,2,3},{-6,2,2,3},{314,0,3,3},{30,1,3,3},{-30,0,4,3},
        {208,0,0,4},{-285,1,0,4},{11,2,0,4},{-504,0,1,4},{504,1,1,4},{-14,2,1,4},{426,0,2,4},{-261,1,2,4},{3,2,2,4},{-148,0,3,4},{42,1,3,4},{18,0,4,4},
        {392,0,0,5},{-105,1,0,5},{7,2,0,5},{-770,0,1,5},{178,1,1,5},{-10,2,1,5},{496,0,2,5},{-85,1,2,5},{3,2,2,5},{-130,0,3,5},{12,1,3,5},{12,0,4,5}
    };
    int programs, upper;
    cin >> programs >> upper;
    mint factorial = 1;
    for (int i = 1; i <= programs; ++i) factorial *= i;
    const array<mint, 5> small_at_three = {1, 6, 36, 114, 210};

    auto answer = holonomic_sequence<mint>(upper + 1, 5,
        [&](int time, int distance) {
            if (distance == 0) {
                return mint{time - 3} * (3 * time - 5) * (2 * time - programs)
                    * (2 * time - programs - 1);
            }
            mint result{};
            for (const auto& [coefficient, n_power, t_power, term_distance] : terms) {
                if (term_distance == distance) {
                    result += coefficient * mint{programs}.pow(n_power) * mint{time}.pow(t_power);
                }
            }
            return result;
        },
        [&](int time, const auto&) -> optional<mint> {
            if (2 * time == programs) return factorial / mint{2}.pow(time);
            if (2 * time == programs + 1) {
                mint result = factorial * (programs + 1) / mint{2}.pow(time);
                if (time >= 2) result += factorial * (time - 1) / mint{2}.pow(time - 2);
                return result;
            }
            if (time == 3) return programs < 5 ? small_at_three[programs] : mint{};
            if (time < 3) return mint{};
            return nullopt;
        });
    for (int time = 1; time <= upper; ++time) cout << answer[time] << '\n';
}
