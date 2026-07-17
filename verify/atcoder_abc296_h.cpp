#define PROBLEM "https://atcoder.jp/contests/abc296/tasks/abc296_h"
#include <bits/stdc++.h>
#include "recipe/connectivityfrontier.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, m;
    cin >> n >> m;
    vector<uint64_t> required(n);
    for (int row = 0; row < n; ++row) {
        string line;
        cin >> line;
        for (int column = 0; column < m; ++column) {
            if (line[column] == '#') required[row] |= uint64_t{1} << column;
        }
    }
    cout << minimum_connected_grid_completion(required, m) << '\n';
}
