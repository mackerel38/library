#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_c"
#include <bits/stdc++.h>
#include "fps/fps.hpp"

using namespace std;
using namespace poe;

int main() {
    int length, maximum, sum;
    cin >> length >> maximum >> sum;
    fps998244353 one(min(maximum, sum) + 1, 1);
    cout << one.pow(length, sum + 1)[sum] << '\n';
}
