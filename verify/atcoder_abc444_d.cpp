#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> length(n);
    int maximum = 0;
    for (int& value : length) cin >> value, maximum = max(maximum, value);
    vector<long long> digit(maximum + 20);
    for (int value : length) ++digit[0], --digit[value];
    for (int position = 1; position < static_cast<int>(digit.size()); ++position) {
        digit[position] += digit[position - 1];
    }
    for (int position = 0; position < static_cast<int>(digit.size()) - 1; ++position) {
        digit[position + 1] += digit[position] / 10;
        digit[position] %= 10;
    }
    while (digit.size() > 1 && digit.back() == 0) digit.pop_back();
    for (auto iterator = digit.rbegin(); iterator != digit.rend(); ++iterator) cout << *iterator;
    cout << '\n';
}
