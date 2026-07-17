#include <bits/stdc++.h>
#include "graph/graph.hpp"
#include "graph/shortestpath.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int modulus, a, b;
    cin >> modulus >> a >> b;
    directed_graph<> reverse_graph(modulus * modulus);
    vector<int> sources;
    for (int first = 0; first < modulus; ++first) {
        for (int second = 0; second < modulus; ++second) {
            const int next = (a * second + b * first) % modulus;
            reverse_graph.add_edge(second * modulus + next, first * modulus + second);
            if (first == 0 || second == 0) sources.push_back(first * modulus + second);
        }
    }
    const auto result = bfs(reverse_graph, sources);
    int answer = 0;
    for (int vertex = 0; vertex < modulus * modulus; ++vertex)
        answer += !result.reachable(vertex);
    cout << answer << '\n';
}
