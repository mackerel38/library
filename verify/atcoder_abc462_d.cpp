#define PROBLEM "https://atcoder.jp/contests/abc462/tasks/abc462_d"
#include <bits/stdc++.h>
#include "structure/prefixsum.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, duration;
    cin >> n >> duration;
    constexpr int maximum_time = 1'000'000;
    differencearray<int> candidates(maximum_time + 1);
    for (int i = 0; i < n; ++i) {
        int enter, leave;
        cin >> enter >> leave;
        if (leave - enter >= duration) {
            candidates.add(enter, leave - duration + 1, 1);
        }
    }
    candidates.build();
    long long answer = 0;
    for (int count : candidates.values()) answer += 1LL * count * (count - 1) / 2;
    cout << answer << '\n';
}
