#include "string/runlength.hpp"

int main() {
    std::mt19937 random(817293);
    for (int trial = 0; trial < 10000; ++trial) {
        const int n = random() % 200;
        std::vector<int> values(n);
        for (int& value : values) value = random() % 8;
        const auto runs = poe::runlength(values);
        assert(poe::runlength_decode(runs) == values);
        int length = 0;
        for (int index = 0; index < static_cast<int>(runs.size()); ++index) {
            assert(runs[index].second > 0);
            if (index > 0) assert(runs[index - 1].first != runs[index].first);
            length += runs[index].second;
        }
        assert(length == n);
    }
}
