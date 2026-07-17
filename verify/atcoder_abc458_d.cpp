#include <bits/stdc++.h>
#include "structure/median.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long initial;
    int q;
    cin >> initial >> q;
    median<long long> values;
    values.add(initial);
    while (q--) {
        long long a, b;
        cin >> a >> b;
        values.add(a);
        values.add(b);
        cout << values.lower() << '\n';
    }
}
