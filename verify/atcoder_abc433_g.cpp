#include <bits/stdc++.h>
#include "game/dag.hpp"
#include "string/suffixautomaton.hpp"
using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int test_count;
    cin >> test_count;
    while (test_count--) {
        string source;
        cin >> source;
        suffixautomaton<char> automaton(source);
        directed_graph<> graph(automaton.size());
        for (int state = 0; state < automaton.size(); ++state) {
            for (const auto& [symbol, next] : automaton[state].next) {
                static_cast<void>(symbol);
                graph.add_edge(state, next);
            }
        }
        cout << (dag_winning_positions(graph)[0] ? "Alice" : "Bob") << '\n';
    }
}
