#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int limit;
    cin >> limit;
    vector<unsigned char> representation_count(limit + 1);
    for (long long first = 1; first * first <= limit; ++first) {
        for (long long second = first + 1;
             first * first + second * second <= limit; ++second) {
            auto& count = representation_count[first * first + second * second];
            if (count < 2) ++count;
        }
    }
    vector<int> answer;
    for (int value = 1; value <= limit; ++value) {
        if (representation_count[value] == 1) answer.push_back(value);
    }
    cout << answer.size() << '\n';
    for (int index = 0; index < static_cast<int>(answer.size()); ++index) {
        if (index) cout << ' ';
        cout << answer[index];
    }
    cout << '\n';
}
