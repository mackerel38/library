#include "tree/subarrayunion.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(20260718);
    const auto next = [&](int bound) {
        return static_cast<int>(random() % static_cast<unsigned int>(bound));
    };
    for (int iteration = 0; iteration < 5000; ++iteration) {
        const int n = 1 + next(10);
        vector<int> parent(n, -1);
        vector<long long> weight(n);
        for (int vertex = 1; vertex < n; ++vertex) parent[vertex] = next(vertex);
        for (auto& value : weight) value = random() % 10;
        const int length = next(10);
        vector<int> vertices(length);
        for (int& vertex : vertices) vertex = next(n);
        long long expected = 0;
        for (int left = 0; left < length; ++left) {
            vector<bool> used(n);
            for (int right = left; right < length; ++right) {
                for (int vertex = vertices[right]; vertex != -1; vertex = parent[vertex]) {
                    used[vertex] = true;
                }
                for (int vertex = 0; vertex < n; ++vertex) {
                    if (used[vertex]) expected += weight[vertex];
                }
            }
        }
        assert(sum_subarray_root_path_unions(parent, vertices, weight) == expected);
    }
}
