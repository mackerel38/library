#include <bits/stdc++.h>
#include "math/math.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_count;
    cin >> test_count;
    while (test_count--) {
        long long n, modulus, a, b;
        cin >> n >> modulus >> a >> b;
        const long long first = floor_sum(n, modulus, a, b);
        const long long second = floor_sum(n, modulus, a - 1, b - 1);
        cout << n - first + second << '\n';
    }
}
