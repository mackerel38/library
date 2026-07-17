#include <bits/stdc++.h>
#include "structure/doubling.hpp"

using namespace std;
using namespace poe;


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int day_count, event_count, query_count;
    cin >> day_count >> event_count >> query_count;
    vector<int> next(day_count + 2, -1);
    for (int i = 0; i < event_count; ++i) {
        int first, last;
        cin >> first >> last;
        const int after = last + 1;
        if (next[first] == -1 || after < next[first]) next[first] = after;
    }
    for (int day = day_count - 1; day >= 1; --day) {
        if (next[day] == -1 || (next[day + 1] != -1 && next[day + 1] < next[day])) {
            next[day] = next[day + 1];
        }
    }
    doubling table(next, day_count + 1);
    while (query_count--) {
        int left, right;
        cin >> left >> right;
        const auto [answer, last] = table.max_jump(
            left, [&](int day) { return day <= right + 1; }
        );
        (void)last;
        cout << answer << '\n';
    }
}
