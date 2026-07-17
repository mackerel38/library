#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> height(n), at_least(q + 1);
    int removed = 0;
    while (q--) {
        int type, value;
        cin >> type >> value;
        if (type == 1) {
            --value;
            ++height[value];
            ++at_least[height[value]];
            if (at_least[height[value]] == n) removed = height[value];
        } else {
            const int raw_height = removed + value;
            cout << (raw_height < static_cast<int>(at_least.size()) ? at_least[raw_height] : 0) << '\n';
        }
    }
}
