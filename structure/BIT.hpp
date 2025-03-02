#line 1 "structure/BIT.hpp"
template <typename T>
struct BIT {
    int n;
    vector<T> data;
    BIT(int _n) : n(_n) {
        data.assign(_n+1, T{});
    }
    BIT(vector<T>& v) : n((int)size(v)) {
        build(v);
    }
    void build(vector<T>& v) {
        assert(n == (int)size(v));
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
    T operator[](int p) { return sum(p+1) - sum(p); }
};