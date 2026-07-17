#include <bits/stdc++.h>
#include "structure/swag.hpp"

using namespace std;
using namespace poe;

namespace {

constexpr long long inf = (1LL << 62);

struct minplusmatrix {
    long long value[2][2];
};

long long add(long long a, long long b) {
    if (a == inf || b == inf) return inf;
    return min(inf, a + b);
}

minplusmatrix compose(minplusmatrix first, minplusmatrix second) {
    minplusmatrix result{{{inf, inf}, {inf, inf}}};
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                result.value[i][j] = min(
                    result.value[i][j], add(second.value[i][k], first.value[k][j]));
            }
        }
    }
    return result;
}

minplusmatrix minplus_identity() {
    return {{{0, inf}, {inf, 0}}};
}

minplusmatrix transition(long long cost) {
    return {{{inf, 0}, {cost, cost}}};
}

}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_count;
    cin >> test_count;
    while (test_count--) {
        int n, k;
        cin >> n >> k;
        vector<long long> cost(n);
        for (long long& value : cost) cin >> value;

        swag<minplusmatrix, compose, minplus_identity> window;
        for (int i = 0; i < k; ++i) window.push(transition(cost[i]));
        long long answer = inf;
        for (int left = 0; left + k <= n; ++left) {
            const auto product = window.prod();
            const long long previous = left == 0 ? inf : cost[left - 1];
            answer = min(answer, min(product.value[1][0], add(product.value[1][1], previous)));
            if (left + k < n) {
                window.pop();
                window.push(transition(cost[left + k]));
            }
        }
        cout << answer << '\n';
    }
}
