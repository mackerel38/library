#define PROBLEM "https://judge.yosupo.jp/problem/suffixarray"

#include "string/suffixarray.hpp"

using namespace std;
using namespace poe;

int main() {
    string text;
    cin >> text;
    const auto suffix_array = suffix_array(text);
    for (size_t i = 0; i < suffix_array.size(); ++i) {
        cout << suffix_array[i] << " \n"[i + 1 == suffix_array.size()];
    }
}
