#include <bits/stdc++.h>
#include "string/palindromictree.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string text;
    cin >> text;
    palindromictree<char> tree(text);
    cout << tree.longest_palindrome().length() << '\n';
}
