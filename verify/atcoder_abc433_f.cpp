#include <bits/stdc++.h>
#include "math/combination.hpp"
#include "math/modint.hpp"
using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string source;
    cin >> source;
    using mint = staticmodint<998244353>;
    combination<mint> choose(source.size());
    array<int, 10> left{}, right{};
    for (char digit : source) ++right[digit - '0'];
    mint answer = 0;
    for (char character : source) {
        const int digit = character - '0';
        --right[digit];
        if (digit < 9) answer += choose.choose(left[digit] + right[digit + 1], left[digit] + 1);
        ++left[digit];
    }
    cout << answer << '\n';
}
