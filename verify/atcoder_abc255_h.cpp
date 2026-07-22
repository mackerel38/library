#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "structure/intervalmap.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unsigned long long n;
    int q;
    cin >> n >> q;
    intervalmap<unsigned long long, unsigned long long> last_day(1, n + 1, 0);
    while (q--) {
        unsigned long long day, left, right;
        cin >> day >> left >> right;
        modint998244353 answer = 0;
        for (const auto& segment : last_day.assign(left, right + 1, day)) {
            const modint998244353 count = segment.right - segment.left;
            const modint998244353 endpoint_sum = segment.left + segment.right - 1;
            const modint998244353 index_sum = count * endpoint_sum / 2;
            answer += index_sum * modint998244353(day - segment.value);
        }
        cout << answer << '\n';
    }
}
