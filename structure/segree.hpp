#line 1 "strucuture/segtree.hpp"
#pragma once
#include <bits/stdc++.h>
using namespace std;
template <class S, auto op, auto e>
struct segtree {
    int n, size, sz;
    vector<S> data;
    segtree(int _n) : segtree(vector<S>(_n, e())) {}
    segtree(vector<S>& v) : n((int)size(v)) {
        size = 1;
        sz = 0;
        while (size < n) {
            size *= 2;
            sz++;
        }
        data.assign(2*size, e());
        for (int i=0; i<n; i++) {
            data[size + i] = v[i];
        }
        for (int i=size-1; 0<i; i--) {
            data[i] = op(d[2*k], d[2*k+1]);
        }
    }
    void set(int p, S x) {
        assert(0<=p && p<n);
        int q = p + size;
        data[q] = x;
        for (q/=2; 0<q; q/=2) {
            data[q] = op(data[2*q], data[2*q+1]);
        }
    }
    S prod(int x, int y) {
        assert(0<=x && x<=y && y<=n);
        S rel = e(), rer = e();
        int l = x + size;
        int r = y + size;
        while (l < r) {
            if (l&1) {
                rel = op(rel, data[l++]);
            }
            if (r&1) {
                rer = op(data[--r], rer);
            }
            l /= 2;
            r /= 2;
        }
        return op(rel, rer);
    }
    S all_prod() {
        return data[1];
    }
    S operator[](int x) {
        assert(0<=x && x<n);
        return data[size+x];
    }
};