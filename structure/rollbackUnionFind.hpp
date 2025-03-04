#line 1 "structure/rollbackUnionFind.hpp"
#pragma once
#include <bits/stdc++.h>
using namespace std;
struct rollbackUnionFind {
    int n, snap;
    vector<int> data;
    stack<pair<int, int>> history;
    rollbackUnionFind(int _n) : n(_n), snap(0), data(_n, -1) {}
    int root(int x) {
        assert(0<=x && x<n);
        if (data[x] < 0) {
            return x;
        } else {
            return root(data[x]);
        }
    }
    int merge(int x, int y) {
        history.push(make_pair(x, data[x]));
        history.push(make_pair(y, data[y]));
        x = root(x);
        y = root(y);
        if (x == y) {
            return false;
        }
        if (data[x] < data[y]) {
            data[x] += data[y];
            data[y] = x;
        } else {
            data[y] += data[x];
            data[x] = y;
        }
        return true;
    }
    void snapshot() {
        snap = (int)history.size();
    }
    int state() {
        return (int)history.size();
    }
    void rollback() {
        rollback(snap);
    }
    void rollback(int state) {
        while (state <(int) history.size()) {
            data[history.top().first] = history.top().second;
            history.pop();
        }
    }
    int unite(int x, int y) {
        return merge(x, y);
    }
    int size(int x) {
        return -data[root(x)];
    }
    int same(int x, int y) {
        return root(x) == root(y);
    }
};