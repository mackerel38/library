#include <bits/stdc++.h>
#include "math/matrix.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, edge_count;
    unsigned long long length;
    cin >> n >> edge_count >> length;
    const int infinity = edge_count + 1;
    matrix<int> edges(n, n);
    for (int from = 0; from < n; ++from) {
        fill(edges[from].begin(), edges[from].end(), infinity);
    }
    for (int time = 1; time <= edge_count; ++time) {
        int from, to;
        cin >> from >> to;
        edges[--from][--to] = time;
    }
    const auto answer = minmax_matrix_power(edges, length, infinity, 0);
    for (int vertex = 0; vertex < n; ++vertex) {
        if (vertex) cout << ' ';
        cout << (answer[0][vertex] == infinity ? -1 : answer[0][vertex]);
    }
    cout << '\n';
}
