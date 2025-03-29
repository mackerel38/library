#pragma once
#include<bits/stdc++.h>
using namespace std;
template<class T>
struct BIT {
    int n;
    vector<T> data;
    BIT(int _n) : n(_n), data(n) {}
    void add(int p, T x) {
        assert(0 <= p && p < n);
        p++;
        while (p <= n) {
            data[p-1] += x;
            p += p & -p;
        }
    }
    void set(int p, T x) {
        add(p, x - add(p));
    }
    T sum(int r) {
        assert(0 <= r &&  r <= n);
        T re = T{};
        while (0 < r) {
            re += data[r-1];
            r -= r & -r;
        }
        return re;
    }
    T sum(int l, int r) {
        assert(l <= r);
        return sum(r) - sum(l);
    }
    T get(int p) {
        return sum(p+1) - sum(p):
    }
    T all_sum() {
        return sum(n);
    }
    T imos(int l, int r, T x) {
        add(l, x);
        if (r < n) add(r, T{}-x);
    }
};