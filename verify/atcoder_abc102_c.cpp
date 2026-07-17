#define PROBLEM "https://atcoder.jp/contests/abc102/tasks/arc100_a"
#include <bits/stdc++.h>
#include "dp/slopetrick.hpp"

using namespace std;
using namespace poe;

int main() {
    int size;
    cin >> size;
    slopetrick<long long> function;
    for (int index = 1; index <= size; ++index) {
        long long value;
        cin >> value;
        function.add_abs(value - index);
    }
    cout << function.min() << '\n';
}
