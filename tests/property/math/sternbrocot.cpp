#include "math/sternbrocot.hpp"

using namespace std;
using namespace poe;

vector<bool> expand(const sternbrocotpath& path) {
    vector<bool> result;
    for (auto [direction, count] : path.runs) {
        while (count--) result.push_back(direction);
    }
    return result;
}

int main() {
    vector<pair<long long, long long>> fractions{{0, 1}, {1, 0}};
    for (int denominator = 1; denominator <= 20; ++denominator) {
        for (int numerator = 1; numerator <= 20; ++numerator) {
            if (gcd(numerator, denominator) == 1) {
                fractions.emplace_back(numerator, denominator);
            }
        }
    }
    vector<vector<bool>> paths;
    for (const auto [numerator, denominator] : fractions) {
        const auto path = stern_brocot_path(numerator, denominator);
        paths.push_back(expand(path));
        if (numerator == 0 || denominator == 0) {
            assert(path.depth == -1 && path.runs.empty());
            continue;
        }
        pair<long long, long long> left{0, 1};
        pair<long long, long long> right{1, 0};
        for (bool direction : paths.back()) {
            const pair<long long, long long> middle{
                left.first + right.first, left.second + right.second};
            if (direction) left = middle;
            else right = middle;
        }
        assert(left.first + right.first == numerator);
        assert(left.second + right.second == denominator);
    }

    mt19937 random(273);
    for (int iteration = 0; iteration < 1000; ++iteration) {
        shuffle(fractions.begin(), fractions.end(), random);
        const int count = 1 + static_cast<int>(random() % 30);
        vector<pair<long long, long long>> points(fractions.begin(), fractions.begin() + count);
        const auto tree = stern_brocot_virtual_tree(points);
        for (int leaf = 0; leaf < count; ++leaf) {
            long long weight = 0;
            for (int vertex = leaf; tree.parent[vertex] != -1; vertex = tree.parent[vertex]) {
                weight += tree.depth[vertex] - tree.depth[tree.parent[vertex]];
            }
            const auto path = stern_brocot_path(points[leaf].first, points[leaf].second);
            assert(weight == path.depth + 1);
        }
        for (int first = 0; first < count; ++first) {
            for (int second = 0; second < count; ++second) {
                int left = first;
                int right = second;
                set<int> ancestors;
                while (left != -1) {
                    ancestors.insert(left);
                    left = tree.parent[left];
                }
                while (!ancestors.contains(right)) right = tree.parent[right];
                long long expected = -1;
                const auto first_path = stern_brocot_path(
                    points[first].first, points[first].second);
                const auto second_path = stern_brocot_path(
                    points[second].first, points[second].second);
                if (first_path.depth >= 0 && second_path.depth >= 0) {
                    const auto a = expand(first_path);
                    const auto b = expand(second_path);
                    expected = 0;
                    while (expected < static_cast<long long>(min(a.size(), b.size())) &&
                           a[expected] == b[expected]) ++expected;
                }
                assert(tree.depth[right] == expected);
            }
        }
    }
}
