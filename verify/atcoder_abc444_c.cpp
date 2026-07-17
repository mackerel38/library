#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> values(n);
    for (long long& value : values) cin >> value;
    ranges::sort(values);

    auto paired = [&](int size, long long sum) {
        if (size % 2) return false;
        for (int left = 0; left < size / 2; ++left) {
            if (values[left] + values[size - 1 - left] != sum) return false;
        }
        return true;
    };

    vector<long long> answer;
    const int smaller = ranges::lower_bound(values, values.back()) - values.begin();
    if (paired(smaller, values.back())) answer.push_back(values.back());
    if (paired(n, values.front() + values.back())) {
        answer.push_back(values.front() + values.back());
    }
    ranges::sort(answer);
    answer.erase(unique(answer.begin(), answer.end()), answer.end());
    for (int i = 0; i < static_cast<int>(answer.size()); ++i) {
        if (i) cout << ' ';
        cout << answer[i];
    }
    cout << '\n';
}
