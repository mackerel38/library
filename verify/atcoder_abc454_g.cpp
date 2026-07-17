#define PROBLEM "https://atcoder.jp/contests/abc454/tasks/abc454_g"
#include <bits/stdc++.h>
#include "tree/dsuontree.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, prefix, color_count;
    unsigned long long state;
    cin >> n >> state >> prefix >> color_count;
    vector<int> given_parent(prefix - 1), given_color(prefix);
    for (int& value : given_parent) cin >> value;
    for (int& value : given_color) cin >> value;

    constexpr unsigned long long mask = (1ULL << 31) - 1;
    auto next_state = [&] {
        state = (state * 1103515245 + 12345) & mask;
    };
    vector<int> parent(n, -1);
    for (int vertex = 1; vertex < n; ++vertex) {
        if (vertex < prefix) parent[vertex] = given_parent[vertex - 1] - 1;
        else {
            parent[vertex] = state % vertex;
            next_state();
        }
    }
    vector<int> color(n);
    for (int vertex = 0; vertex < n; ++vertex) {
        if (vertex < prefix) color[vertex] = given_color[vertex] - 1;
        else {
            color[vertex] = state % color_count;
            next_state();
        }
    }

    dsuontree tree(std::move(parent));
    vector<int> count(color_count), frequency(n + 1);
    int maximum = 0;
    constexpr long long mod = 998244353;
    long long answer = 0;
    auto add = [&](int vertex) {
        const int current_color = color[vertex];
        const int old = count[current_color]++;
        if (old > 0) --frequency[old];
        ++frequency[old + 1];
        maximum = max(maximum, old + 1);
    };
    auto remove = [&](int vertex) {
        const int current_color = color[vertex];
        const int old = count[current_color]--;
        --frequency[old];
        if (old > 1) ++frequency[old - 1];
        while (maximum > 0 && frequency[maximum] == 0) --maximum;
    };
    auto query = [&](int vertex) {
        const long long index = vertex + 1;
        answer += (maximum ^ index) * (static_cast<long long>(frequency[maximum]) ^ index) % mod;
        if (answer >= mod) answer -= mod;
    };
    tree.run(add, remove, query);
    cout << answer << '\n';
}
