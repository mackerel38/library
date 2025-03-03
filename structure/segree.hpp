#line 1 "strucuture/segtree.hpp"
template <class S, auto op, auto e>
struct segtree {
    int n, size, sz;
    vector<S> data;
    segtree(int _n) : segtree(vector<S>(_n, e())) {}
    segtree(vector<S>& v) : n((int)size(v)) {
        size = 1;
        sz = 0;
        while (n < size) {
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
        p += size;
        data[p] = x;
        for (int i=1; i<=log; i++) {
            d[p>>i]
        }
    }
};