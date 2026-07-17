#include "fps/combinatorial.hpp"

using namespace std;
using namespace poe;

template<class Mint>
Mint brute(int n, const vector<Mint>& weight) {
    const int length = n - 2;
    vector<int> code(length);
    Mint answer = 0;
    const auto enumerate = [&](this auto&& self, int index) -> void {
        if (index != length) {
            for (int vertex = 0; vertex < n; ++vertex) {
                code[index] = vertex;
                self(index + 1);
            }
            return;
        }
        vector<int> degree(n, 1);
        for (const int vertex : code) ++degree[vertex];
        Mint value = 1;
        for (const int current : degree) value *= weight[current];
        answer += value;
    };
    enumerate(0);
    return answer;
}

int main() {
    mt19937 random(20260716);
    for (int n = 2; n <= 7; ++n) {
        for (int trial = 0; trial < 100; ++trial) {
            vector<modint998244353> weight(n);
            for (int degree = 1; degree < n; ++degree) weight[degree] = random() % 6;
            assert(labeled_tree_degree_weight_sum<998244353>(n, weight) == brute(n, weight));

            vector<int> allowed;
            for (int degree = 1; degree < n; ++degree) {
                if (random() % 2) allowed.push_back(degree);
            }
            vector<modint998244353> indicator(n);
            for (const int degree : allowed) indicator[degree] = 1;
            assert(count_labeled_trees_with_degrees<998244353>(n, allowed) == brute(n, indicator));
        }
    }
}
