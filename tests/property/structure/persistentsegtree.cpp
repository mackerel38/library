#include "structure/persistentsegtree.hpp"

long long add(long long left, long long right) { return left + right; }
long long zero() { return 0; }

int main() {
    std::mt19937 random(20260715);
    for (int n = 1; n <= 45; ++n) {
        std::vector<long long> initial(n);
        for (long long& value : initial) value = static_cast<int>(random() % 31) - 15;
        poe::persistentsegtree<long long, add, zero> tree(initial);
        std::vector<int> roots = {tree.initial()};
        std::vector<std::vector<long long>> versions = {initial};
        for (int step = 0; step < 1000; ++step) {
            const int source = random() % roots.size();
            if (random() % 2 == 0) {
                const int index = random() % n;
                const long long value = static_cast<int>(random() % 101) - 50;
                auto next = versions[source];
                next[index] = value;
                roots.push_back(tree.set(roots[source], index, value));
                versions.push_back(std::move(next));
            } else {
                int left = random() % (n + 1), right = random() % (n + 1);
                if (left > right) std::swap(left, right);
                const long long expected = std::accumulate(
                    versions[source].begin() + left, versions[source].begin() + right, 0LL);
                assert(tree.prod(roots[source], left, right) == expected);
            }
        }
    }
}
