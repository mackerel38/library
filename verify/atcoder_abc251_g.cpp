#include <bits/stdc++.h>
#include "geometry/polygon.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<point<long long>> polygon(n);
    for (auto& [x, y] : polygon) cin >> x >> y;
    int m;
    cin >> m;
    vector<point<long long>> shifts(m);
    for (auto& [x, y] : shifts) cin >> x >> y;
    const translatedpolygonintersection region(polygon, shifts);
    int q;
    cin >> q;
    while (q--) {
        point<long long> target;
        cin >> target.x >> target.y;
        cout << (region.contains(target) ? "Yes" : "No") << '\n';
    }
}
