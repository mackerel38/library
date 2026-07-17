#define PROBLEM "https://atcoder.jp/contests/abc418/tasks/abc418_g"
#include <bits/stdc++.h>
#include "dp/partition.hpp"
#include "string/automaton.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    string text;
    cin >> n >> text;
    for (int truth_table = 0; truth_table < 16; ++truth_table) {
        vector operation(2, vector<int>(2));
        operation[0][0] = truth_table >> 3 & 1;
        operation[0][1] = truth_table >> 2 & 1;
        operation[1][0] = truth_table >> 1 & 1;
        operation[1][1] = truth_table & 1;
        auto beautiful = [&](const vector<int>& word) {
            return !word.empty() && (parenthesized_results(word, operation) >> 1 & 1);
        };
        const auto automaton = right_context_dfa(2, 3, beautiful);
        const auto answer = accepted_substring_stats(
            automaton, text, [](char symbol) { return symbol - '0'; }
        );
        cout << answer.longest << ' ' << answer.count << '\n';
    }
}
