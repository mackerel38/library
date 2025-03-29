#pragma once
#include<bits/stdc++.h>
using namespace std;
template<class S, auto op, auto e>
struct segtree {
    int n, size, sz;
    vector<S> data;
    segtree(int _n) : segtree(vector<S>(_n, e())) {}
    segtree(vector<S>& v) : n(v.size()) {
        size = bit_ceil((unsigned int)n);
        sz = __builtin_ctz((unsigned int)size);
        data = vector<S>(2 * size, e());
        for (int i=0; i<n; i++) data[size+i] = v[i];
        for (int i=size-1; 0<i; i--) update(i);
    }
    void set(int p, S x) {
        assert(0 <= p && p < n);
        for (p+=size; 0<p; p>>=1) update(p);
    }
    S get(int p) {
        assert(0 <= p && p < n);
        return data[size+p];
    }
    S prod(int l, int r) {
        assert(0 <= l && l <= r && r <= n);
        S ll = e(), rr = e();
        l += size;
        r += size;
        while (l < r) {
            if (l & 1) ll = op(ll, d[l++]);
            if (r & 1) rr = op(d[--r], rr);
            l >>= 1;
            r >>= 1;
        }
        return op(ll, rr);
    }
    S all_prod() {
        return data[1];
    }
    void update(int p) {
        d[p] = op(d[2*p], d[2*p+1]);
    }
};