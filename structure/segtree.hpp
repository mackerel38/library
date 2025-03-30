#pragma once
#include<bits/stdc++.h>
using namespace std;
template<class S, auto op>
struct segtree {
    int n, size, sz;
    S e;
    vector<S> data;
    // 大きさn, 単位元e(省略するとS{} になる) のセグ木を構築 O(n)
    segtree(int _n, S _e = S{}) : n(_n), e(_e) { build(vector<S>(_n, e)); }
    // 大きさv.size(), 単位元e(省略するとS{} になる) のセグ木を構築 O(n)
    segtree(vector<S>& v, S _e = S{}) : n(v.size()), e(_e) { build(v); }
    void build(vector<S> v) {
        size = __bit_ceil((unsigned int)n);
        data.assign(2 * size, e);
        for (int i=0; i<n; i++) data[size+i] = v[i];
        for (int i=size-1; 0<i; i--) update(i);
    }
    // p 番目の要素をx にする O(log n)
    void set(int p, S x) {
        assert(0 <= p && p < n);
        p += size;
        data[p] = x;
        for (p>>=1; 0<p; p>>=1) update(p);
    }
    // p 番目の要素を取得する O(1)
    S get(int p) {
        assert(0 <= p && p < n);
        return data[size+p];
    }
    // [l, r) の区間クエリに答える O(log n)
    S prod(int l, int r) {
        assert(0 <= l && l <= r && r <= n);
        S ll = e, rr = e;
        l += size;
        r += size;
        while (l < r) {
            if (l & 1) ll = op(ll, data[l++]);
            if (r & 1) rr = op(data[--r], rr);
            l >>= 1;
            r >>= 1;
        }
        return op(ll, rr);
    }
    // [0, n) のクエリに答える O(1)
    S all_prod() {
        return data[1];
    }
    void update(int p) {
        data[p] = op(data[2*p], data[2*p+1]);
    }
};