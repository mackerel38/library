#include <bits/stdc++.h>
#include "graph/poset.hpp"
#include "string/palindromictree.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string text;
    cin >> text;
    palindromictree tree(text);
    vector<string> palindromes;
    for (int state = 2; state < tree.size(); ++state) {
        const auto value = tree.palindrome(state);
        palindromes.emplace_back(value.begin(), value.end());
    }
    const auto result = maximum_poset_antichain(
        palindromes.size(),
        [&](int left, int right) {
            return palindromes[left].size() < palindromes[right].size()
                && palindromes[right].find(palindromes[left]) != string::npos;
        }
    );
    cout << result.size() << '\n';
}
