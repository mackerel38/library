#include "algorithm/dominance.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(266);
    for (int iteration = 0; iteration < 5000; ++iteration) {
        const int n = 1 + static_cast<int>(random() % 14);
        vector<weightedpoint3d<int, long long>> points(n);
        set<tuple<int, int, int>> used;
        for (auto& point : points) {
            do {
                point.x = static_cast<int>(random() % 8);
                point.y = static_cast<int>(random() % 8);
                point.z = static_cast<int>(random() % 8);
            } while (!used.emplace(point.x, point.y, point.z).second);
            point.weight = random() % 20;
        }

        long long expected = 0;
        for (int mask = 0; mask < (1 << n); ++mask) {
            vector<int> selected;
            long long weight = 0;
            for (int index = 0; index < n; ++index) {
                if (mask >> index & 1) {
                    selected.push_back(index);
                    weight += points[index].weight;
                }
            }
            ranges::sort(selected, {}, [&](int index) {
                return tuple{points[index].x, points[index].y, points[index].z};
            });
            bool chain = true;
            for (int i = 1; i < static_cast<int>(selected.size()); ++i) {
                const auto& previous = points[selected[i - 1]];
                const auto& current = points[selected[i]];
                chain &= previous.x <= current.x;
                chain &= previous.y <= current.y;
                chain &= previous.z <= current.z;
            }
            if (chain) expected = max(expected, weight);
        }

        const auto result = maximum_weight_dominance_chain_3d(points);
        assert(result.weight == expected);
        long long restored_weight = 0;
        for (int i = 0; i < static_cast<int>(result.indices.size()); ++i) {
            const auto& current = points[result.indices[i]];
            restored_weight += current.weight;
            if (i > 0) {
                const auto& previous = points[result.indices[i - 1]];
                assert(previous.x <= current.x && previous.y <= current.y && previous.z <= current.z);
            }
        }
        assert(restored_weight == result.weight);
    }
}
