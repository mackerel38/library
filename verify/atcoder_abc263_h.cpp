#include <bits/stdc++.h>
#include "geometry/line.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long k;
    cin >> n >> k;
    vector<line<long long>> lines(n);
    for (auto& [a, b, c] : lines) cin >> a >> b >> c;
    cout << fixed << setprecision(15)
         << kth_line_intersection_distance(lines, k, 50, 3'000'000) << '\n';
}
