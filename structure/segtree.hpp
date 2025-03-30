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
    // p 番目の要素を取得する O(1)
    S operator[](int p) {
        return get(p);
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
    vector<S> values() {
        vector<S> re(n);
        for (int i=0; i<n; i++) re[i] = data[size+i];
        return re;
    }
    void update(int p) {
        data[p] = op(data[2*p], data[2*p+1]);
    }
    // f(op([l, r)))=true となる最大のr を返す O(log n)
    template<auto f>
    int max_right(int l=0) {
        assert(f(e));
        assert(0 <= l && l <= n);
        if (l == n) return l;
        l += size;
        S s = e;
        do {
            while (l % 2 == 0) l >>= 1;
            if (!f(op(s, data[l]))) {
                while (l < size) {
                    l = 2 * l;
                    if (f(op(s, data[l]))) s = op(s, data[l++]);
                }
                return l - size;
            }
            s = op(s, data[l]);
            l++;
        } while (l != l & -l);
        return n;
    }
    // f(op([l, r)))=true となる最小のl を返す O(log n)
    template<auto f>
    int min_left(int r=n) {
        assert(f(e));
        assert(0 <= r && r <= n);
        if (r == 0) return r;
        r += size;
        S s = e;
        do {
            r--;
            while (r % 2 == 1) r >>= 1;
            if (!f(op(data[r], s))) {
                while (r < size) {
                    r = 2 * r + 1;
                    if (f(op(data[r], s))) s = op(data[r--], s);
                }
                return (r + 1) - size;
            }
            s = op(data[r], s);
        } while(r != r & -r);
        return 0;
    }
};
