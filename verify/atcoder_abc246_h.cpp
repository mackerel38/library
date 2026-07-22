#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "string/dynamicsubsequence.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    string text;
    cin >> n >> q >> text;
    const auto mask = [](char character) -> uint64_t {
        if (character == '0') return 1;
        if (character == '1') return 2;
        return 3;
    };
    vector<uint64_t> allowed(n);
    transform(text.begin(), text.end(), allowed.begin(), mask);
    dynamicsubsequencecount<modint998244353, 2> sequence(allowed);
    while (q--) {
        int index;
        char character;
        cin >> index >> character;
        sequence.set(index - 1, mask(character));
        cout << sequence.count() << '\n';
    }
}
