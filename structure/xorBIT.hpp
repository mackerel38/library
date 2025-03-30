#pragma once
#include<bits/stdc++.h>
using namespace std;
template<class T>
struct xorBIT {
    int n;
    vector<T> data;
    // xorBIT を構築する O(n)
    BIT(int _n) : n(_n), data(n) {}
    // p 番目の値にx を加算する O(log n)
    void add(int p, T x) {
        assert(0 <= p && p < n);
        p++;
        while (p <= n) {
            data[p-1] ^= x;
            p += p & -p;
        }
    }
    // p 番目の値をx にする O(log n)
    void set(int p, T x) {
        add(p, x ^ get(p));
    }
    // [0, r) の総xor を求める O(log n)
    T sum(int r) {
        assert(0 <= r &&  r <= n);
        T re = T{};
        while (0 < r) {
            re ^= data[r-1];
            r -= r & -r;
        }
        return re;
    }
    // [l, r) の総xor を求める O(log n)
    T sum(int l, int r) {
        assert(l <= r);
        return sum(r) ^ sum(l);
    }
    // p 番目の値を取得する O(log n)
    T get(int p) {
        return sum(p+1) ^ sum(p);
    }
    T operator[](int p) {
        return get(p);
    }
    // 全体の総xor を取得する O(log n)
    T all_sum() {
        return sum(n);
    }
    // [l, r) にx を加算する 一点取得はsum(p) で行う 区間xor クエリができなくなるので注意O(log n)
    void imos(int l, int r, T x) {
        add(l, x);
        if (r < n) add(r, x);
    }
};