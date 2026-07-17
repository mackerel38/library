#include <cassert>
#include <random>
#include <vector>

#include "structure/doubling.hpp"

int main() {
    std::mt19937 random(438);
    for (int size = 1; size <= 20; ++size) {
        for (int trial = 0; trial < 30; ++trial) {
            std::vector<int> next(size);
            std::vector<long long> weight(size);
            for (int i = 0; i < size; ++i) {
                next[i] = random() % size;
                weight[i] = int(random() % 21) - 10;
            }
            poe::weighteddoubling table(next, weight, 1000);
            for (int start = 0; start < size; ++start) {
                int vertex = start;
                long long sum = 0;
                for (int step = 0; step <= 1000; ++step) {
                    if (step <= 20 || step % 37 == 0 || step == 1000) {
                        const auto result = table.jump(start, step);
                        assert(result.vertex == vertex && result.aggregate == sum);
                    }
                    sum += weight[vertex];
                    vertex = next[vertex];
                }
            }
        }
    }
}
