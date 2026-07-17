#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_count;
    cin >> test_count;
    while (test_count--) {
        string s;
        cin >> s;
        array<int, 26> count{};
        for (const char c : s) ++count[c - 'a'];
        if (*max_element(count.begin(), count.end()) > (static_cast<int>(s.size()) + 1) / 2) {
            cout << "No\n";
            continue;
        }
        priority_queue<pair<int, char>> queue;
        for (int c = 0; c < 26; ++c) if (count[c]) queue.push({count[c], static_cast<char>('a' + c)});
        string answer;
        while (!queue.empty()) {
            auto [frequency, c] = queue.top();
            queue.pop();
            if (!answer.empty() && answer.back() == c) {
                auto [other_frequency, other] = queue.top();
                queue.pop();
                answer += other;
                if (--other_frequency) queue.push({other_frequency, other});
                queue.push({frequency, c});
            } else {
                answer += c;
                if (--frequency) queue.push({frequency, c});
            }
        }
        cout << "Yes\n" << answer << '\n';
    }
}
