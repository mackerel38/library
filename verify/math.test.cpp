#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_floor_of_linear"

#include "math/math.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int test_count;
    cin >> test_count;
    while (test_count--) {
        long long n, m, a, b;
        cin >> n >> m >> a >> b;
        cout << floor_sum(n, m, a, b) << '\n';
    }
}
