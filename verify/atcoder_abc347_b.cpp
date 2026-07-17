#include <bits/stdc++.h>
#include "string/suffixautomaton.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string text;
    cin >> text;
    suffixautomaton<char> automaton(text);
    cout << automaton.distinct_substrings() << '\n';
}
