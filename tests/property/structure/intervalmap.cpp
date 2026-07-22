#include "structure/intervalmap.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(2551);
    for (int trial = 0; trial < 1000; ++trial) {
        const int n = 1 + random() % 40;
        vector<int> values(n, 0);
        intervalmap<int, int> data(0, n, 0);
        for (int query = 0; query < 300; ++query) {
            int left = random() % (n + 1), right = random() % (n + 1);
            if (left > right) swap(left, right);
            const int value = random() % 6;
            const auto removed = data.assign(left, right, value);
            int covered = 0;
            for (const auto& segment : removed) {
                for (int index = segment.left; index < segment.right; ++index) {
                    assert(segment.value == values[index]);
                }
                covered += segment.right - segment.left;
            }
            assert(covered == right - left);
            fill(values.begin() + left, values.begin() + right, value);
            for (int index = 0; index < n; ++index) assert(data[index] == values[index]);
            int expected_segments = 1;
            for (int index = 1; index < n; ++index) expected_segments += values[index] != values[index - 1];
            assert(data.segment_count() == expected_segments);
        }
    }
}
