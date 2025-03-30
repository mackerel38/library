#pragma once
#include<bits/stdc++.h>
using namespace std;
template<class S, auto op, auto F, auto mapping, auto composition>
struct lazysegtree {
    int n, size, sz;
    S e;
    F id;
    vector<S> data;
    vector<S> lazy;
    // 遅延伝播セグメント木を構築 O(n)
    lazysegtree(int n, S e(), F id()) : n(n), e(e), id(id) {
        sz = 0;
        while ((1 << sz) < n) sz++;
        size = 1 << sz;
        data.assign(2 * size, e);
        lazy.assign(size, id);
    }
    // 遅延伝播セグメント木を構築 O(n)
    lazysegtree(vector<S>& v, S e(), F id()) : lazysegtree(v.size(), e, id) {
        for (int i=0; i<n; i++) data[size + i] = v[i];
        for (int k=size-1; 0<k; k--) update(k);
    }
    void update(int k) {
        data[k] = op(data[2*k], data[2*k+1]);
        if (lazy[k] != id) {
            data[k] = mapping(lazy[k], data[k]);
            if (k < size) {
                lazy[2*k] = composition(lazy[k], lazy[2*k]);
                lazy[2*k+1] = composition(lazy[k], lazy[2*k+1]);
            }
            lazy[k] = id;
        }
    }
    // 遅延伝播を適用する O(log n)
    void apply(int k, F x) {
        assert(0 <= k && k < size);
        data[k] = mapping(x, data[k]);
        if (k < size) {
            lazy[k] = composition(x, lazy[k]);
        }
    }
    // 値の変更 O(log n)
    void set(int p, S x) {
        assert(0 <= p && p < n);
        p += size;
        data[p] = x;
        for (int k=p>>1; 0<k; k>>=1) {
            update(k);
        }
    }
    S get(int p) {
        assert(0 <= p && p < n);
        p += size;
        for (int k=sz; 0<k; k--) {
            update(p >> k);
        }
        return data[p];
    }
    // 区間[l, r)の値を取得 O(log n)
    S prod(int l, int r) {
        assert(0 <= l && l <= r && r <= n);
        if (l == r) return e;
        l += size;
        r += size;
        for (int k=sz; 0<k; k--) {
            if (((l >> k) << k) != l) update(l >> k);
            if (((r >> k) << k) != r) update(r >> k);
        }
        S resl = e, resr = e;
        while (l < r) {
            if (l & 1) resl = op(resl, data[l++]);
            if (r & 1) resr = op(data[--r], resr);
            l >>= 1;
            r >>= 1;
        }
        return op(resl, resr);
    }
    // 区間[l, r)に遅延伝播を適用する O(log n)
    void apply(int l, int r, S x) {
        assert(0 <= l && l <= r && r <= n);
        if (l == r) return;
        l += size;
        r += size;
        for (int k=sz; 0<k; k--) {
            if (((l >> k) << k) != l) update(l >> k);
            if (((r >> k) << k) != r) update(r >> k);
        }
        while (l < r) {
            if (l & 1) apply(l++, x);
            if (r & 1) apply(--r, x);
            l >>= 1;
            r >>= 1;
        }
    }
    // 区間[0, n) の値を取得 O(1)
    S all_prod() {
        return data[1];
    }
    // f(op([l, r)))=true となる最大のr を返す O(log n)
    template<auto f>
    int max_right(int l) {
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
            s = op(s, data[l++]);
        } while ((l & -l) != l);
        return n;
    }
    // f(op([l, r)))=true となる最小のl を返す O(log n)
    template<auto f>
    int min_left(int r) {
        assert(f(e));
        assert(0 <= r && r <= n);
        if (r == 0) return r;
        r += size;
        S s = e;
        do {
            r--;
            while (r > 1 && r % 2 == 1) r >>= 1;
            if (!f(op(data[r], s))) {
                while (r < size) {
                    if (f(op(data[2*r], s))) s = op(data[2*r], s);
                    r = 2 * r + 1;
                }
                return r - size + 1;
            }
            s = op(data[r--], s);
        } while ((r & -r) != r);
        return 0;
    }
};