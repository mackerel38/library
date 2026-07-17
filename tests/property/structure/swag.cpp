#include "structure/swag.hpp"

std::string concatenate(const std::string& left, const std::string& right) {
    return left + right;
}

std::string empty_string() {
    return {};
}

int main() {
    std::mt19937 random(18273);
    poe::swag<std::string, concatenate, empty_string> actual;
    std::deque<std::string> expected;
    for (int operation = 0; operation < 20000; ++operation) {
        if (expected.empty() || random() % 3 != 0) {
            std::string value(1, 'a' + random() % 26);
            actual.push(value);
            expected.push_back(value);
        } else {
            assert(actual.front() == expected.front());
            actual.pop();
            expected.pop_front();
        }
        std::string aggregate;
        for (const auto& value : expected) aggregate += value;
        assert(actual.prod() == aggregate);
        assert(actual.size() == static_cast<int>(expected.size()));
        assert(actual.empty() == expected.empty());
        if (!expected.empty()) assert(actual.back() == expected.back());
    }
}
