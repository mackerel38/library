#include <cassert>
#include <random>
#include "string/stacksequence.hpp"

int main() {
    using mint = poe::staticmodint<998244353>;
    std::mt19937 random(20260716);
    for (int trial = 0; trial < 300; ++trial) {
        const int n = 1 + random() % 80;
        std::vector<int> operations(n);
        for (int& operation : operations) operation = random() % 3 == 0 ? -1 : random() % 10;
        poe::stacksequence<998244353, 10> sequence(operations);
        for (int query = 0; query < 500; ++query) {
            if (random() % 3 == 0) {
                const int index = random() % n;
                operations[index] = random() % 3 == 0 ? -1 : random() % 10;
                sequence.set(index, operations[index]);
                assert(sequence[index] == operations[index]);
                continue;
            }
            int left = random() % (n + 1), right = random() % (n + 1);
            if (left > right) std::swap(left, right);
            int unmatched = 0;
            std::vector<int> stack;
            for (int i = left; i < right; ++i) {
                if (operations[i] == -1) {
                    if (stack.empty()) ++unmatched;
                    else stack.pop_back();
                } else stack.push_back(operations[i]);
            }
            mint value = 0;
            for (int digit : stack) value = value * 10 + digit;
            const auto actual = sequence.prod(left, right);
            assert(actual.pop == unmatched);
            assert(actual.length == static_cast<int>(stack.size()));
            assert(actual.value == value);
        }
    }
}
