#define PROBLEM "https://atcoder.jp/contests/abc427/tasks/abc427_g"
#include <bits/stdc++.h>
#include "structure/thresholdsequence.hpp"

using namespace std;
using namespace poe;

int main() {
    int n;
    long long up, down;
    cin >> n >> up >> down;
    thresholdsequence<long long> sequence(up, down);
    while (n--) {
        long long threshold;
        cin >> threshold;
        sequence.append(threshold);
    }

    int queries;
    cin >> queries;
    while (queries--) {
        int type;
        long long value;
        cin >> type >> value;
        if (type == 1) sequence.append(value);
        else cout << sequence.apply(value) << '\n';
    }
}
