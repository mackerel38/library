#line 1 "structure/rollbackUnionFind.hpp"
#pragma once
#include <bits/stdc++.h>
using namespace std;
// 巻き戻し可能なUnionFind
struct rollbackUnionFind {
    int n, snap;
    vector<int> data;
    stack<pair<int, int>> history;
    rollbackUnionFind(int _n) : n(_n), snap(0), data(_n, -1) {}
    // 親を取得 O(log n)
    int root(int x) {
        assert(0<=x && x<n);
        if (data[x] < 0) {
            return x;
        } else {
            return root(data[x]);
        }
    }
    // 併合 O(log n)
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
    // 現在の状態を保存 O(1)
    void snapshot() {
        snap = (int)history.size();
    }
    // 状態の番号を取得 O(1)
    int state() {
        return (int)history.size();
    }
    // 保存した位置まで巻き戻し 巻き戻し回数をmとして、O(m)
    void rollback() {
        rollback(snap);
    }
    // 指定した状態まで巻き戻し 巻き戻し回数をmとして、O(m)
    void rollback(int state) {
        assert(state <= (int)history.size());
        while (state < (int)history.size()) {
            data[history.top().first] = history.top().second;
            history.pop();
        }
    }
    // mergeと同じ
    int unite(int x, int y) {
        return merge(x, y);
    }
    // 要素が属する集合の大きさを取得 O(log n)
    int size(int x) {
        return -data[root(x)];
    }
    // 要素が同じ集合に属するか判定 O(log n)
    int same(int x, int y) {
        return root(x) == root(y);
    }
};