#define PROBLEM "https://judge.yosupo.jp/problem/factorize"
#include <bits/stdc++.h>
#include "math/prime.hpp"

using namespace std;
using namespace poe;

int main() {
    int queries;
    cin >> queries;
    while (queries--) {
        unsigned long long value;
        cin >> value;
        vector<unsigned long long> answer;
        for (const auto [prime, exponent] : factorize(value)) {
            for (int count = 0; count < exponent; ++count) answer.push_back(prime);
        }
        cout << answer.size();
        for (const auto prime : answer) cout << ' ' << prime;
        cout << '\n';
    }
}
