#include <bits/stdc++.h>
#include "graph/functionalgraph.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> next(n);
    for (int& vertex : next) {
        cin >> vertex;
        --vertex;
    }
    functionalgraph graph(next);
    int answer = 0;
    for (int id = 0; id < graph.cycle_count(); ++id) {
        answer += graph.cycle(id).size();
    }
    cout << answer << '\n';
}
