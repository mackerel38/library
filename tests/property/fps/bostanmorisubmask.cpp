#include "fps/bostanmori.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(20260716);
    using mint = modint998244353;
    for (int trial = 0; trial < 1000; ++trial) {
        const int order = 1 + random() % 20;
        vector<mint> numerator(order), denominator(order + 1);
        denominator[0] = 1;
        for (mint& value : numerator) value = random() % 100;
        for (int index = 1; index <= order; ++index) {
            denominator[index] = random() % 100;
        }
        const unsigned long long mask = random() % 1024;
        vector<mint> coefficient(1024);
        for (int index = 0; index < 1024; ++index) {
            coefficient[index] = index < order ? numerator[index] : mint{};
            for (int shift = 1; shift <= order && shift <= index; ++shift) {
                coefficient[index] -= denominator[shift] * coefficient[index - shift];
            }
        }
        mint expected = 0;
        for (unsigned long long index = mask;; index = (index - 1) & mask) {
            expected += coefficient[index];
            if (index == 0) break;
        }
        assert(bostan_mori_submask_sum<998244353>(numerator, denominator, mask) == expected);
    }
}
