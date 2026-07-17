#define PROBLEM "https://atcoder.jp/contests/abc430/tasks/abc430_c"
#include <bits/stdc++.h>
#include "algorithm/twopointer.hpp"

using namespace std;
using namespace poe;

int main() {
    int n, minimum_a, maximum_b;
    string text;
    cin >> n >> minimum_a >> maximum_b >> text;
    cout << count_subarrays_frequency_constraints(
        text, minimum_a, maximum_b,
        [](char symbol) { return symbol == 'a'; },
        [](char symbol) { return symbol == 'b'; }
    ) << '\n';
}
