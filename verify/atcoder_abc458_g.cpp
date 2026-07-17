#include <bits/stdc++.h>
#include "algorithm/binarysearch.hpp"
#include "dp/concave.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_count;
    cin >> test_count;
    while (test_count--) {
        int n;
        cin >> n;
        vector<long long> add(n), survive(n), escape(n);
        for (int i = 0; i < n; ++i) cin >> add[i] >> survive[i] >> escape[i];

        auto possible = [&](long long people) {
            if (people == 0) return true;
            concavesequence dp(people, 0);
            for (int day = 0; day < n; ++day) {
                dp.add_affine(-survive[day], add[day]);
                dp.trim_below(0);
                if (dp.empty()) return false;
                dp.suffix_max(escape[day], 0);
                dp.trim_below(0);
                if (!dp.empty() && dp.left() == 0) return true;
            }
            return false;
        };
        cout << *last_true<long long>(0, add[0] + 1, possible) << '\n';
    }
}
