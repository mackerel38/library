#include <bits/stdc++.h>
#include "algorithm/groupreduce.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<int> values(n);
    for (int& value : values) cin >> value;
    cout << reduce_adjacent_groups(values, 4).size() << '\n';
}
