#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod"

#include "math/convolution.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    vector<long long> a(n), b(m);
    for (long long& value : a) {
        cin >> value;
    }
    for (long long& value : b) {
        cin >> value;
    }
    const auto result = convolution(a, b);
    for (size_t i = 0; i < result.size(); ++i) {
        cout << result[i] << " \n"[i + 1 == result.size()];
    }
}
