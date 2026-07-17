#define PROBLEM "https://atcoder.jp/contests/abc428/tasks/abc428_f"
#include <bits/stdc++.h>
#include "structure/nestedintervals.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    vector<long long> width(n);
    for (auto& value : width) cin >> value;
    nestedintervals<long long> intervals(width);
    int q;
    cin >> q;
    while (q--) {
        int type;
        long long value;
        cin >> type >> value;
        if (type == 1) intervals.align_left(static_cast<int>(value) - 1);
        else if (type == 2) intervals.align_right(static_cast<int>(value) - 1);
        else cout << intervals.count_covering(value) << '\n';
    }
}
