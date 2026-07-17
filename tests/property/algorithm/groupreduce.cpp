#include <cassert>
#include <set>
#include <vector>

#include "algorithm/groupreduce.hpp"

using sequence = std::vector<int>;

static void enumerate(const sequence& values, int group_size, std::set<sequence>& finals) {
    bool moved = false;
    for (int left = 0; left + group_size <= static_cast<int>(values.size()); ++left) {
        bool equal = true;
        for (int i = 1; i < group_size; ++i) equal &= values[left] == values[left + i];
        if (!equal) continue;
        moved = true;
        sequence next = values;
        next.erase(next.begin() + left, next.begin() + left + group_size);
        enumerate(next, group_size, finals);
    }
    if (!moved) finals.insert(values);
}

int main() {
    for (int size = 0; size <= 9; ++size) {
        for (int mask = 0; mask < (1 << size); ++mask) {
            sequence values(size);
            for (int i = 0; i < size; ++i) values[i] = mask >> i & 1;
            for (int group_size = 2; group_size <= 4; ++group_size) {
                std::set<sequence> finals;
                enumerate(values, group_size, finals);
                assert(finals.size() == 1);
                assert(*finals.begin() == poe::reduce_adjacent_groups(values, group_size));
            }
        }
    }
}
