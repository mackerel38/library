#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> value(n);
    for (int& x : value) cin >> x;
    vector<vector<int>> graph(n);
    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        --u;
        --v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    struct event {
        int vertex;
        int parent;
        bool exit;
        bool duplicate;
    };
    vector<bool> answer(n);
    set<int> path_values;
    int duplicates = 0;
    vector<event> stack{{0, -1, false, false}};
    while (!stack.empty()) {
        const auto [vertex, parent, exit, duplicate] = stack.back();
        stack.pop_back();
        if (exit) {
            if (duplicate) --duplicates;
            else path_values.erase(value[vertex]);
            continue;
        }

        const bool already_exists = path_values.contains(value[vertex]);
        if (already_exists) ++duplicates;
        else path_values.insert(value[vertex]);
        answer[vertex] = duplicates > 0;
        stack.push_back({vertex, parent, true, already_exists});
        for (auto iterator = graph[vertex].rbegin(); iterator != graph[vertex].rend(); ++iterator) {
            if (*iterator != parent) stack.push_back({*iterator, vertex, false, false});
        }
    }

    for (bool duplicated : answer) cout << (duplicated ? "Yes" : "No") << '\n';
}
