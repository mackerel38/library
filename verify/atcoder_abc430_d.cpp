#define PROBLEM "https://atcoder.jp/contests/abc430/tasks/abc430_d"
#include <bits/stdc++.h>
#include "structure/nearestset.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    cin >> n;
    nearestset<int> points;
    points.add(0);
    while (n--) {
        int coordinate;
        cin >> coordinate;
        points.add(coordinate);
        cout << points.sum() << '\n';
    }
}
