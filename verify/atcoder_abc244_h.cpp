#include <bits/stdc++.h>
#include "geometry/incrementalhull.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;
    incrementalconvexhull<long long> hull;
    while (q--) {
        long long x, y, a, b;
        cin >> x >> y >> a >> b;
        hull.add({x, y});
        cout << static_cast<long long>(hull.max_dot(a, b)) << '\n';
    }
}
