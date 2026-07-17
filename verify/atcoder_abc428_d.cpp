#define PROBLEM "https://atcoder.jp/contests/abc428/tasks/abc428_d"
#include <bits/stdc++.h>
#include "math/math.hpp"

using namespace std;
using namespace poe;

int main() {
    int test_count;
    cin >> test_count;
    while (test_count--) {
        unsigned long long c, limit;
        cin >> c >> limit;
        unsigned long long answer = 0;
        unsigned long long lower = 1;
        unsigned long long power = 10;
        while (lower <= c + limit) {
            const unsigned long long upper = power - 1;
            const unsigned long long first = max(lower, c + 1);
            const unsigned long long last = min(upper, c + limit);
            if (first <= last) {
                answer += floor_sqrt(c * power + last)
                        - floor_sqrt(c * power + first - 1);
            }
            lower = power;
            power *= 10;
        }
        cout << answer << '\n';
    }
}
