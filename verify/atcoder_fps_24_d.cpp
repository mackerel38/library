#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_d"
#include <bits/stdc++.h>
#include "fps/fps.hpp"
#include "math/combination.hpp"

using namespace std;
using namespace poe;

int main() {
    int length, maximum;
    cin >> length >> maximum;
    const int degree = maximum - (length - 1);
    if (degree < 0) {
        cout << 0 << '\n';
        return 0;
    }
    fps998244353 positive = fps998244353{1, 1}.pow(length - 1, degree + 1).inv(degree + 1);
    fps998244353 negative = fps998244353{1, -1}.pow(length + 1, degree + 1).inv(degree + 1);
    combination<modint998244353> comb(max(length, degree));
    cout << (positive * negative).prefix(degree + 1)[degree] * comb.factorial(length) << '\n';
}
