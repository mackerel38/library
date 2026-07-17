#include <bits/stdc++.h>
#include "algorithm/shortestword.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int modulus;
    cin >> modulus;
    const auto answer = shortest_lexicographic_word<char>(
        10 * modulus,
        0,
        [](int state) { return state != 0 && state / 10 == 0; },
        [&](int state, auto relax) {
            const int remainder = state / 10;
            const int last = state % 10;
            for (int digit = max(1, last); digit <= 9; ++digit) {
                const int next_remainder = (10LL * remainder + digit) % modulus;
                if (!relax(10 * next_remainder + digit, static_cast<char>('0' + digit))) break;
            }
        }
    );
    if (!answer) {
        cout << -1 << '\n';
    } else {
        for (char digit : *answer) cout << digit;
        cout << '\n';
    }
}
