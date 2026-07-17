#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long k;
    cin >> n >> k;
    vector<vector<long long>> sequences(n);
    for (auto& sequence : sequences) {
        int length;
        cin >> length;
        sequence.resize(length);
        for (auto& value : sequence) cin >> value;
    }
    vector<long long> repeat(n);
    for (auto& count : repeat) cin >> count;
    --k;
    for (int i = 0; i < n; ++i) {
        const __int128 block = static_cast<__int128>(sequences[i].size()) * repeat[i];
        if (k < block) {
            cout << sequences[i][k % sequences[i].size()] << '\n';
            return 0;
        }
        k -= static_cast<long long>(block);
    }
}
