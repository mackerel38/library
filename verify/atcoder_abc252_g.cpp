#include <bits/stdc++.h>
#include "math/modint.hpp"
#include "tree/preorder.hpp"

using namespace std;
using namespace poe;

using mint = staticmodint<998244353>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> preorder(n);
    for (int& vertex : preorder) cin >> vertex;
    cout << count_sorted_child_preorder_trees<mint>(preorder).val() << '\n';
}
