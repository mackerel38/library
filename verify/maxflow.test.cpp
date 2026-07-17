#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/GRL_6_A"

#include "flow/maxflow.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    maxflow<long long> graph(n);
    while (m--) {
        int from, to;
        long long cap;
        cin >> from >> to >> cap;
        graph.add_edge(from, to, cap);
    }
    cout << graph.flow(0, n - 1) << '\n';
}
