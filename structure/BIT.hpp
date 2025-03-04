#line 1 "structure/BIT.hpp"
#pragma once
#include <bits/stdc++.h>
using namespace std;
template <class T>
struct BIT {
    int n, sz;
    vector<T> data;
    BIT(int _n) : n(_n), data(_n+1, T{}) {
        sz = 1;
        while ((1<<sz) < n) {
            sz++;
        }
    }
    BIT(vector<T>& v) : n((int)size(v)), data((int)size(v)+1, T{}) {
        sz = 1;
        while ((1<<sz) < n) {
            sz++;
        }
        for (int i=1; i<=n; i++) {
            data[i] = v[i-1];
        }
        for (int i=1; i<=n; i++) {
            int j = i + (i & -i);
            if (j <= n) {
                data[j] += data[i];
            }
        }
    }
    void add(int p, T x) {
        assert(0<=p && p<n);
        for (int j=p+1; j<=n; j+=j&-j) {
            data[j] += x;
        }
    }
    void set(int p, T x) {
        add(int p, x - (sum(x+1) - sum(x)));
    }
    void imos(int l, int r, T x) {
        add(l, x);
        if (r < n) {
            add(r, -x);
        }
    }
    T sum(int p) {
        assert(0<=p && p<=n);
        T re = T{};
        for (int j=p; 0<j; j-=j&-j) {
            re += data[j];
        }
        return re;
    }
    T sum(int l, int r) {
        return sum(r) - sum(l);
    }
    T all_sum() {
        return sum(n);
    }
    T operator[](int p) {
        return sum(p+1) - sum(p);
    }
    int lower_bound(T x) {
        int re = 0;
        int k = sz;
        T y = T{};
        for (; 0<k; k>>=1) {
            if (n < re + k) {
                continue;
            }
            if (y+data[re+k] < x) {
                y += data[re+k];
                re += k;
            }
        }
        return re;
    }
    int upper_bound(T x) {
        int re = 0;
        int k = sz;
        T y = T{};
        for (; 0<k; k>>=1) {
            if (n < re + k) {
                continue;
            }
            if (y+data[re+k] <= x) {
                y += data[re+k];
                re += k;
            }
        }
        return re;
    }
};
