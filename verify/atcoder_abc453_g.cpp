#define PROBLEM "https://atcoder.jp/contests/abc453/tasks/abc453_g"
#include <bits/stdc++.h>
#include "structure/persistentsegtree.hpp"

using namespace std;
using namespace poe;

long long add(long long left, long long right) { return left + right; }
long long zero() { return 0; }

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int sequence_count, length, query_count;
    cin >> sequence_count >> length >> query_count;
    persistentsegtree<long long, add, zero> tree(length);
    tree.reserve_nodes(2 * length + query_count * 20);
    vector<int> root(sequence_count, tree.initial());
    while (query_count--) {
        int type, x;
        cin >> type >> x;
        --x;
        if (type == 1) {
            int y;
            cin >> y;
            root[x] = root[--y];
        } else if (type == 2) {
            int index;
            long long value;
            cin >> index >> value;
            root[x] = tree.set(root[x], --index, value);
        } else {
            int left, right;
            cin >> left >> right;
            cout << tree.prod(root[x], left - 1, right) << '\n';
        }
    }
}
