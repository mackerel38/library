#define PROBLEM "https://atcoder.jp/contests/fps-24/tasks/fps_24_x"
#include <bits/stdc++.h>
#include "fps/composition.hpp"

using namespace std;
using namespace poe;

int main() {
    int size;
    cin >> size;
    fps998244353 function(size);
    for (auto& coefficient : function) cin >> coefficient;
    const auto answer = functional_power<998244353>(function, (998244353LL + 1) / 2, size);
    for (int i = 0; i < size; ++i) cout << answer[i] << (i + 1 == size ? '\n' : ' ');
}
