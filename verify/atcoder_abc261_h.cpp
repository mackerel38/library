#include <bits/stdc++.h>
#include "game/alternating.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, start;
    cin >> n >> m >> start;
    directed_graph<long long> graph(n);
    graph.read(m, 1);
    const auto result = alternating_path_game(graph);
    const auto& answer = result.value(start - 1, gameplayer::minimize);
    if (answer) cout << *answer << '\n';
    else cout << "INFINITY\n";
}
