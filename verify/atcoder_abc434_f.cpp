#include <bits/stdc++.h>
#include "string/concatenation.hpp"
using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int test_count;
    cin >> test_count;
    while (test_count--) {
        int n;
        cin >> n;
        vector<string> strings(n);
        for (auto& value : strings) cin >> value;
        cout << second_concatenation(strings) << '\n';
    }
}
