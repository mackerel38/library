#define PROBLEM "https://atcoder.jp/contests/abc417/tasks/abc417_g"
#include <bits/stdc++.h>
#include "string/persistentsequence.hpp"

using namespace std;
using namespace poe;

int main() {
    int query_count;
    cin >> query_count;
    persistentconcatenation<char> sequences({'0', '1'}, 1'000'000'000'000'000'000ULL);
    while (query_count--) {
        int left, right;
        unsigned long long index;
        cin >> left >> right >> index;
        const int version = sequences.add(left, right);
        cout << sequences.get(version, index - 1) << '\n';
    }
}
