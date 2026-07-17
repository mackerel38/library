#include <algorithm>
#include <cassert>
#include <random>
#include <string>

#include "string/bracketsequence.hpp"

int main() {
    std::mt19937 random(428);
    poe::bracketsequence brackets;
    std::string brute;
    std::vector<int> balances{0};
    std::vector<int> minimums{0};
    for (int query = 0; query < 100000; ++query) {
        if (brute.empty() || random() % 3 != 0) {
            const char value = random() & 1 ? '(' : ')';
            brackets.push(value);
            brute.push_back(value);
            balances.push_back(balances.back() + (value == '(' ? 1 : -1));
            minimums.push_back(std::min(minimums.back(), balances.back()));
        } else {
            brackets.pop();
            brute.pop_back();
            balances.pop_back();
            minimums.pop_back();
        }
        assert(brackets.size() == static_cast<int>(brute.size()));
        assert(brackets.string() == brute);
        assert(brackets.balance() == balances.back());
        assert(brackets.minimum_balance() == minimums.back());
        assert(brackets.good() == (balances.back() == 0 && minimums.back() == 0));
        if (query % 1000 == 0) {
            int balance = 0;
            int minimum = 0;
            for (const char value : brute) {
                balance += value == '(' ? 1 : -1;
                minimum = std::min(minimum, balance);
            }
            assert(balance == balances.back() && minimum == minimums.back());
        }
    }
}
