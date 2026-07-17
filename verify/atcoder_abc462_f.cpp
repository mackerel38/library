#define PROBLEM "https://atcoder.jp/contests/abc462/tasks/abc462_f"
#include <bits/stdc++.h>
#include "dp/sequence.hpp"

using namespace std;
using namespace poe;

int main() {
    int test_count;
    cin >> test_count;
    while (test_count--) {
        string text;
        int increase;
        cin >> text >> increase;
        cout << minimum_unbordered_pattern_increase(text, string("ABC"), increase) << '\n';
    }
}
