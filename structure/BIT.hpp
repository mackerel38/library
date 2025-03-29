#pragma once
#include<bits/stdc++.h>
using namespace std;
template<class T>
struct BIT {
    int n;
    vector<T> data;
    // BIT を0-indexed で構築する。O(n)
    BIT(int _n) : n(_n), data(n) {}
    // p 番目の値にx を加算する。O(log n)
    void add(int p, T x) {
        assert(0 <= p && p < n);
        p++;
        while (p <= n) {
            data[p-1] += x;
            p += p & -p;
        }
    }
    // p 番目の値をx にする。O(log n)
    void set(int p, T x) {
        add(p, x - add(p));
    }
    // [0, r) の総和を求める。O(log n)
    T sum(int r) {
        assert(0 <= r &&  r <= n);
        T re = T{};
        while (0 < r) {
            re += data[r-1];
            r -= r & -r;
        }
        return re;
    }
    // [l, r) の総和を求める。O(log n)
    T sum(int l, int r) {
        assert(l <= r);
        return sum(r) - sum(l);
    }
    // p 番目の値を取得する。O(log n)
    T get(int p) {
        return sum(p+1) - sum(p);
    }
    // 全体の総和を取得する。O(log n)
    T all_sum() {
        return sum(n);
    }
    // [l, r) にx を加算する。一点取得はsum(p) で行う。O(log n)
    T imos(int l, int r, T x) {
        add(l, x);
        if (r < n) add(r, T{}-x);
    }
    // x <= sum(p) となる最小のp を求める。O(log n)
    int lower_bound(T x) {
        if (x <= 0) return 0;
        int re = 0, t = 1;
        while (t < n) t <<= 1;
        while (t) {
            if (re + t < n && data[re+t] < x) {
                x -= data[re+t];
                re += t;
                t >>= 1;
            }
        }
        return re;
    }
    // x < sum(p) となる最小のp を求める。O(log n)
    int upper_bound(T x) {
        if (x < 0) return 0;
        int re = 0, t = 1;
        while (t < n) t <<= 1;
        while (t) {
            if (re + t < n && data[re+t] <= x) {
                x -= data[re+t];
                re += t;
                t >>= 1;
            }
        }
        return re;
    }
};