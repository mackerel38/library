#include "graph/unlabeledcount.hpp"
#include "math/modint.hpp"

namespace {

std::string canonical(int n, int graph, const std::vector<int>& colors) {
    std::vector<int> permutation(n);
    std::iota(permutation.begin(), permutation.end(), 0);
    std::string best;
    do {
        std::string current;
        for (int i = 0; i < n; ++i) current.push_back(char('a' + colors[permutation[i]]));
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                int first = permutation[i];
                int second = permutation[j];
                if (first > second) std::swap(first, second);
                const int index = first * (2 * n - first - 1) / 2 + second - first - 1;
                current.push_back(char('0' + ((graph >> index) & 1)));
            }
        }
        if (best.empty() || current < best) best = std::move(current);
    } while (std::next_permutation(permutation.begin(), permutation.end()));
    return best;
}

long long brute(int n, int k) {
    const int edge_count = n * (n - 1) / 2;
    int colorings = 1;
    for (int i = 0; i < n; ++i) colorings *= k;
    std::set<std::string> classes;
    for (int encoded = 0; encoded < colorings; ++encoded) {
        int value = encoded;
        std::vector<int> colors(n);
        int used = 0;
        for (int& color : colors) {
            color = value % k;
            value /= k;
            used |= 1 << color;
        }
        if (used != (1 << k) - 1) continue;
        for (int graph = 0; graph < (1 << edge_count); ++graph) {
            classes.insert(canonical(n, graph, colors));
        }
    }
    return classes.size();
}

}

int main() {
    using mint = poe::staticmodint<1000000007>;
    for (int n = 1; n <= 4; ++n) {
        for (int k = 1; k <= n; ++k) {
            assert(poe::count_surjectively_colored_unlabeled_graphs<mint>(n, k)
                   == mint{brute(n, k)});
        }
    }
}
