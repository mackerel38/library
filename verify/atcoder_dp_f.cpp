#define PROBLEM "https://atcoder.jp/contests/dp/tasks/dp_f"
#include <bits/stdc++.h>
#include "dp/sequence.hpp"

using namespace std;
using namespace poe;

int main() {
    string left, right;
    cin >> left >> right;
    cout << lcs(left, right) << '\n';
}
