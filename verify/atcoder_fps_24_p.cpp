#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_p"
#include <bits/stdc++.h>
#include "fps/polynomial.hpp"
#include "math/combination.hpp"

using namespace std;
using namespace poe;

int main() {
    int balls, maximum_box, limit;
    cin >> balls >> maximum_box >> limit;
    combination<modint998244353> comb(balls);
    fps998244353 polynomial(balls + 1);
    for (int in_box_zero = 0; in_box_zero <= limit; ++in_box_zero) {
        polynomial[balls - in_box_zero] = comb.choose(balls, in_box_zero);
    }
    vector<modint998244353> points(maximum_box);
    iota(points.begin(), points.end(), modint998244353{1});
    for (const auto answer : multipoint_evaluation(polynomial, points)) cout << answer << '\n';
}
