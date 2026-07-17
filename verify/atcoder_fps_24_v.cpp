#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_v"
#include <bits/stdc++.h>
#include "fps/fps.hpp"

using namespace std;
using namespace poe;

int main() {
    int steps, horizontal, vertical;
    cin >> steps >> horizontal >> vertical;
    horizontal = abs(horizontal);
    vertical = abs(vertical);
    using mint = modint998244353;
    using series = fps998244353;

    vector<mint> factorial(steps + 1, 1), inverse_factorial(steps + 1, 1);
    for (int i = 1; i <= steps; ++i) factorial[i] = factorial[i - 1] * i;
    inverse_factorial[steps] = factorial[steps].inv();
    for (int i = steps; i > 0; --i) inverse_factorial[i - 1] = inverse_factorial[i] * i;

    auto one_axis = [&](int displacement) {
        series left(steps + 1), right(steps + 1);
        for (int i = 0; i + displacement <= steps; ++i) {
            left[i] = inverse_factorial[i] * inverse_factorial[i + displacement]
                * inverse_factorial[i + displacement];
        }
        for (int i = displacement; i <= steps; ++i) {
            right[i] = inverse_factorial[i] * inverse_factorial[i - displacement]
                * inverse_factorial[i - displacement];
        }
        series result = (left * right).prefix(steps + 1);
        for (int k = 0; k <= steps; ++k) result[k] *= factorial[k] * factorial[k];
        return result;
    };

    series x = one_axis(horizontal);
    series y = one_axis(vertical);
    series unused(steps + 1);
    mint power = 1;
    for (int i = 0; i <= steps; ++i) {
        unused[i] = power * inverse_factorial[i];
        power *= -4;
    }
    cout << ((x * y).prefix(steps + 1) * unused).coeff(steps) * factorial[steps] << '\n';
}
