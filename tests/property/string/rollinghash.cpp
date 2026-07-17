#include "string/rollinghash.hpp"

int main() {
    std::mt19937 random(912783);
    for (int trial = 0; trial < 1000; ++trial) {
        const int first_size = random() % 100;
        const int second_size = random() % 100;
        std::vector<int> first(first_size), second(second_size);
        for (int& value : first) value = random() % 10;
        for (int& value : second) value = random() % 10;
        poe::rollinghash<int> first_hash(first), second_hash(second);
        for (int query = 0; query < 100; ++query) {
            const int first_left = random() % (first_size + 1);
            const int second_left = random() % (second_size + 1);
            const int maximum = std::min(first_size - first_left, second_size - second_left);
            int expected_lcp = 0;
            while (expected_lcp < maximum &&
                   first[first_left + expected_lcp] == second[second_left + expected_lcp]) {
                ++expected_lcp;
            }
            assert(first_hash.lcp(first_left, first_size, second_hash, second_left,
                                  second_size) == expected_lcp);
            const int length = maximum == 0 ? 0 : random() % (maximum + 1);
            const bool expected_equal =
                std::equal(first.begin() + first_left,
                           first.begin() + first_left + length,
                           second.begin() + second_left);
            assert(first_hash.equal(first_left, first_left + length, second_hash,
                                    second_left, second_left + length) == expected_equal);
        }
    }
}
