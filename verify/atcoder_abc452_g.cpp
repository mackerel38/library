#include <bits/stdc++.h>

#include "string/distinctsubstring.hpp"
#include "string/runlength.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<int> a(n);
    for (int& value : a) cin >> value;

    vector<int> encoded;
    for (auto [value, length] : runlength(a)) {
        if (value > length) {
            encoded.push_back(0);
        } else if (value == length) {
            encoded.push_back(value);
        } else {
            encoded.insert(encoded.end(), {value, 0, value});
        }
    }
    cout << count_distinct_substrings_without(encoded, 0) << '\n';
}
