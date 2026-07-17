#define PROBLEM "https://atcoder.jp/contests/abc430/tasks/abc430_e"
#include <bits/stdc++.h>
#include "string/rotation.hpp"

using namespace std;
using namespace poe;

int main() {
    int test_count;
    cin >> test_count;
    while (test_count--) {
        string source, target;
        cin >> source >> target;
        cout << rotation_distance(source, target) << '\n';
    }
}
