#define PROBLEM "https://atcoder.jp/contests/abc430/tasks/abc430_f"
#include <bits/stdc++.h>
#include "algorithm/permutation.hpp"

using namespace std;
using namespace poe;

int main() {
    int test_count;
    cin >> test_count;
    while (test_count--) {
        int n;
        string directions;
        cin >> n >> directions;
        const auto answer = path_order_positions(directions, [](char relation) {
            return relation == 'R';
        });
        for (const int count : answer.counts) cout << count << ' ';
        cout << '\n';
    }
}
