#include <bits/stdc++.h>
#include "string/increasingsubstrings.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string text;
    cin >> n >> text;
    assert(static_cast<int>(text.size()) == n);
    cout << maximum_increasing_disjoint_substrings(text) << '\n';
}
