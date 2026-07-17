#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_count;
    cin >> test_count;
    while (test_count--) {
        int n;
        long long upper;
        cin >> n >> upper;
        vector<long long> moduli(n);
        for (auto& modulus : moduli) cin >> modulus;
        priority_queue<pair<long long, long long>> intervals;
        intervals.push({upper + 1, 1});
        for (const long long modulus : moduli) {
            while (!intervals.empty() && intervals.top().first > modulus) {
                auto [end, count] = intervals.top();
                intervals.pop();
                while (!intervals.empty() && intervals.top().first == end) {
                    count += intervals.top().second;
                    intervals.pop();
                }
                intervals.push({modulus, end / modulus * count});
                if (end % modulus) intervals.push({end % modulus, count});
            }
        }
        long long answer = -1;
        while (!intervals.empty()) {
            answer += intervals.top().second;
            intervals.pop();
        }
        cout << answer << '\n';
    }
}
