#include <cassert>

#include "string/bracketsequence.hpp"

int main() {
    poe::bracketsequence brackets("()");
    assert(brackets.good() && brackets.size() == 2);
    assert(brackets[0] == '(' && brackets.back() == ')');
    brackets.pop();
    assert(!brackets.good() && brackets.balance() == 1);
    brackets.push('(');
    brackets.push(')');
    assert(brackets.string() == "(()");
    assert(brackets.minimum_balance() == 0);
}
