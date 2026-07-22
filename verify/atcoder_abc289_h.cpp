#include <bits/stdc++.h>
#include "fps/fps.hpp"
#include "math/randomwalk.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int first, second, third, time;
    cin >> first >> second >> third >> time;
    const auto meeting = symmetric_walk_meeting_probabilities<998244353>(
        {first, second, third}, time
    );
    const auto returning = symmetric_walk_meeting_probabilities<998244353>(
        {0, 0, 0}, time
    );
    const fps998244353 first_meeting = fps998244353(meeting).div(
        fps998244353(returning), time + 1
    );
    cout << first_meeting[time] << '\n';
}
