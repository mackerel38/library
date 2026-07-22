#include <bits/stdc++.h>
#include "string/increasingsubstrings.hpp"

using namespace std;
using namespace poe;

int brute(string_view text) {
    struct occurrence {
        string value;
        int left;
        int right;
    };
    vector<occurrence> candidates;
    for (int left = 0; left < static_cast<int>(text.size()); ++left) {
        string value;
        for (int right = left; right < static_cast<int>(text.size()); ++right) {
            value += text[right];
            candidates.push_back({value, left, right});
        }
    }
    ranges::sort(candidates, {}, &occurrence::value);
    vector<int> best(text.size());
    int answer = 0;
    for (int first = 0; first < static_cast<int>(candidates.size());) {
        int last = first;
        while (last < static_cast<int>(candidates.size())
               && candidates[last].value == candidates[first].value) {
            ++last;
        }
        vector<int> values(last - first);
        for (int i = first; i < last; ++i) {
            int previous = 0;
            for (int end = 0; end < candidates[i].left; ++end) {
                previous = max(previous, best[end]);
            }
            values[i - first] = previous + 1;
            answer = max(answer, previous + 1);
        }
        for (int i = first; i < last; ++i) {
            best[candidates[i].right] = max(best[candidates[i].right], values[i - first]);
        }
        first = last;
    }
    return answer;
}

int main() {
    for (int size = 0; size <= 12; ++size) {
        for (int mask = 0; mask < (1 << size); ++mask) {
            string text(size, '0');
            for (int i = 0; i < size; ++i) if (mask >> i & 1) text[i] = '1';
            assert(maximum_increasing_disjoint_substrings(text) == brute(text));
        }
    }
    mt19937 random(0);
    for (int trial = 0; trial < 3000; ++trial) {
        string text(random() % 14, 'a');
        for (char& character : text) character += random() % 4;
        assert(maximum_increasing_disjoint_substrings(text) == brute(text));
    }
}
