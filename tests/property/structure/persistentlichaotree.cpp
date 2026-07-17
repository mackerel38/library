#include <algorithm>
#include <cassert>
#include <cmath>
#include <random>
#include <vector>

#include "structure/persistentlichaotree.hpp"

int main() {
    std::mt19937 random(20260716);
    poe::persistentlichaotree<double, poe::lichaomode::maximum> tree(-10.0, 10.0, 55);
    std::vector<int> roots{tree.initial()};
    std::vector<poe::line<double>> lines;
    for (int i = 0; i < 300; ++i) {
        lines.push_back({static_cast<int>(random() % 101) - 50.0,
                         static_cast<int>(random() % 101) - 50.0});
        roots.push_back(tree.add_line(roots.back(), lines.back()));
        for (int query = 0; query < 20; ++query) {
            const double x = (static_cast<int>(random() % 20001) - 10000) / 1000.0;
            const int version = 1 + random() % (roots.size() - 1);
            double brute = -std::numeric_limits<double>::infinity();
            for (int index = 0; index < version; ++index)
                brute = std::max(brute, lines[index].get(x));
            assert(std::abs(tree.get(roots[version], x) - brute) < 1e-8);
        }
    }
}
