#include "string/substringorder.hpp"

struct naiveoccurrence {
    std::string value;
    int string_index;
    int left;
    int right;
};

int main() {
    std::mt19937 random(280);
    for (int iteration = 0; iteration < 5000; ++iteration) {
        const int count = random() % 5;
        std::vector<std::string> strings(count);
        std::vector<naiveoccurrence> expected;
        for (int index = 0; index < count; ++index) {
            strings[index].resize(random() % 7);
            for (char& character : strings[index]) {
                character = static_cast<char>('a' + random() % 3);
            }
            for (int left = 0; left < static_cast<int>(strings[index].size()); ++left) {
                for (int right = left + 1;
                     right <= static_cast<int>(strings[index].size()); ++right) {
                    expected.push_back({strings[index].substr(left, right - left), index,
                                        left, right});
                }
            }
        }
        std::sort(expected.begin(), expected.end(),
                  [](const auto& left, const auto& right) {
                      return left.value < right.value;
                  });

        const poe::substringorder order(strings);
        assert(order.size() == static_cast<long long>(expected.size()));
        for (int rank = 0; rank < static_cast<int>(expected.size()); ++rank) {
            const auto occurrence = order[rank];
            assert(0 <= occurrence.string_index && occurrence.string_index < count);
            assert(0 <= occurrence.left && occurrence.left < occurrence.right);
            assert(occurrence.right <=
                   static_cast<int>(strings[occurrence.string_index].size()));
            const auto actual = strings[occurrence.string_index].substr(
                occurrence.left, occurrence.right - occurrence.left);
            assert(actual == expected[rank].value);
        }
    }
}
