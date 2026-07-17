#include <bits/stdc++.h>
#include "math/matrix.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    unsigned long long steps;
    cin >> n >> steps;
    matrix<long long> cost(n, n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) cin >> cost[i][j];
    const auto answer = minplus_matrix_power(cost, steps);
    for (int vertex = 0; vertex < n; ++vertex) cout << answer[vertex][vertex] << '\n';
}
