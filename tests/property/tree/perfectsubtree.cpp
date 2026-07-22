#include "tree/perfectsubtree.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(264);
    for (int iteration = 0; iteration < 3000; ++iteration) {
        const int n = 1 + static_cast<int>(random() % 12);
        vector<int> parent(n, -1), depth(n);
        for (int vertex = 1; vertex < n; ++vertex) {
            parent[vertex] = static_cast<int>(random() % vertex);
            depth[vertex] = depth[parent[vertex]] + 1;
        }
        for (const int arity : {2, 3}) {
            vector<modint998244353> expected(n);
            for (int prefix = 1; prefix <= n; ++prefix) {
                for (int mask = 1; mask < (1 << prefix); mask += 2) {
                    bool valid = true;
                    int leaf_depth = -1;
                    vector<int> child_count(prefix);
                    for (int vertex = 1; vertex < prefix; ++vertex) {
                        if (!(mask >> vertex & 1)) continue;
                        if (!(mask >> parent[vertex] & 1)) valid = false;
                        else ++child_count[parent[vertex]];
                    }
                    for (int vertex = 0; vertex < prefix; ++vertex) {
                        if (!(mask >> vertex & 1)) continue;
                        if (child_count[vertex] == 0) {
                            if (leaf_depth == -1) leaf_depth = depth[vertex];
                            else valid &= leaf_depth == depth[vertex];
                        } else {
                            valid &= child_count[vertex] == arity;
                        }
                    }
                    if (valid) ++expected[prefix - 1];
                }
            }
            assert(perfect_ary_subtree_prefix_counts<998244353>(parent, arity) == expected);
        }
    }
}
