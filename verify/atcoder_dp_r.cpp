#define PROBLEM "https://atcoder.jp/contests/dp/tasks/dp_r"
#include <bits/stdc++.h>
#include "math/matrix.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    int vertices;
    long long length;
    cin >> vertices >> length;
    matrix<modint1000000007> transition(vertices, vertices);
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            int value;
            cin >> value;
            transition[i][j] = value;
        }
    }
    const auto powered = transition.pow(length);
    modint1000000007 answer;
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) answer += powered[i][j];
    }
    cout << answer.val() << '\n';
}
