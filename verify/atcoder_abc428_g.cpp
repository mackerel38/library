#define PROBLEM "https://atcoder.jp/contests/abc428/tasks/abc428_g"
#include <bits/stdc++.h>
#include "math/dirichlet.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, upper;
    cin >> n >> upper;
    vector<modint998244353> frequency(upper + 1);
    while (n--) {
        int beauty;
        cin >> beauty;
        ++frequency[beauty];
    }
    const auto answer = product_necklace_counts(frequency);
    for (int product = 2; product <= upper; ++product) {
        if (product > 2) cout << ' ';
        cout << answer[product];
    }
    cout << '\n';
}
