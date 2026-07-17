#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_s"
#include <bits/stdc++.h>
#include "fps/implicit.hpp"

using namespace std;
using namespace poe;

int main() {
    int upper, type;
    cin >> upper >> type;
    using mint = modint998244353;
    using series = fps998244353;

    if (type == 1) {
        series total(upper + 1);
        mint factorial = 1;
        for (int vertices = 1; vertices <= upper; ++vertices) {
            factorial *= vertices;
            total[vertices] = mint{vertices}.pow(vertices - 1) / factorial;
        }
        const series alice = newton_fps<998244353>(upper + 1, 0,
            [&](const series& current, int size) {
                const series exponential = (total.prefix(size) - current).exp(size);
                series rooted(size);
                for (int i = 1; i < size; ++i) rooted[i] = exponential[i - 1];
                series derivative = rooted;
                derivative[0] += 1;
                return pair{current - rooted, derivative};
            });
        factorial = 1;
        for (int vertices = 1; vertices <= upper; ++vertices) {
            factorial *= vertices;
            if (vertices >= 2) cout << alice[vertices] * factorial / vertices << '\n';
        }
    } else {
        mint perfect_matchings = 1;
        for (int vertices = 2; vertices <= upper; ++vertices) {
            mint answer = mint{vertices}.pow(vertices - 2);
            if (vertices % 2 == 0) {
                const int pairs = vertices / 2;
                perfect_matchings *= vertices - 1;
                mint contracted = 1;
                if (pairs >= 2) contracted = mint{pairs}.pow(pairs - 2) * mint{4}.pow(pairs - 1);
                answer -= perfect_matchings * contracted;
            }
            cout << answer << '\n';
        }
    }
}
