#include <bits/stdc++.h>

using namespace std;

pair<string, vector<int>> decompose(const string& source) {
    string fixed;
    vector<int> gaps;
    int count = 0;
    for (char character : source) {
        if (character == 'A') ++count;
        else {
            fixed += character;
            gaps.push_back(count);
            count = 0;
        }
    }
    gaps.push_back(count);
    return {fixed, gaps};
}

int main() {
    string source, target;
    cin >> source >> target;
    const auto [source_fixed, source_gaps] = decompose(source);
    const auto [target_fixed, target_gaps] = decompose(target);
    if (source_fixed != target_fixed) {
        cout << -1 << '\n';
        return 0;
    }
    long long answer = 0;
    for (int i = 0; i < static_cast<int>(source_gaps.size()); ++i)
        answer += abs(source_gaps[i] - target_gaps[i]);
    cout << answer << '\n';
}
