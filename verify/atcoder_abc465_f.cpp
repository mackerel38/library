#include <bits/stdc++.h>
#include "structure/prefixsum.hpp"

using namespace std;
using namespace poe;

vector<int> digits(const string& value, int added = 0) {
    vector<int> result;
    result.reserve(value.size());
    for (char digit : value) result.push_back(digit - '0' + added);
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> values(1'000'000);
    while (n--) {
        string id;
        long long value;
        cin >> id >> value;
        values[stoi(id)] = value;
    }
    multidimensionalprefixsum sums(vector<int>(6, 10), std::move(values));
    int query_count;
    cin >> query_count;
    while (query_count--) {
        string lower, upper;
        cin >> lower >> upper;
        bool empty = false;
        for (int index = 0; index < 6; ++index) empty |= lower[index] > upper[index];
        if (empty) cout << 0 << '\n';
        else cout << sums.sum(digits(lower), digits(upper, 1)) << '\n';
    }
}
