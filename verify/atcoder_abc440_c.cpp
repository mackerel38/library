#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_count;
    cin >> test_count;
    while (test_count--) {
        int n, width;
        cin >> n >> width;
        vector<long long> residue_sum(2 * width);
        for (int index = 0; index < n; ++index) {
            long long cost;
            cin >> cost;
            residue_sum[(index + 1) % (2 * width)] += cost;
        }
        long long current = accumulate(residue_sum.begin(), residue_sum.begin() + width, 0LL);
        long long answer = current;
        for (int start = 1; start < 2 * width; ++start) {
            current -= residue_sum[start - 1];
            current += residue_sum[(start + width - 1) % (2 * width)];
            answer = min(answer, current);
        }
        cout << answer << '\n';
    }
}
