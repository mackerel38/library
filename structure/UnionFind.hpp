#line 1 "structure/UnionFind.hpp"
struct UnionFind {
    int n;
    vector<int> data;
    Unionfind(int _n) : n(_n), data(_n, -1) {}
    int root(int x) {
        assert(0<=x && x<n);
        if (data[x] < 0) {
            return x;
        } else {
            data[x] = root(data[x]);
            return data[x];
        }
    }
    int merge(int x, int y) {
        if (same(x, y)) {
            return false;
        }
        if (data[x] < data[y]) {
            data[x] += data[y];
            data[y] = x;
        } else {
            data[y] += data[x];
            data[x] = y;
        }
        return true;
    }
    int unite(int x, int y) {
        return merge(x, y);
    }
    int size(int x) {
        return -data[root(x)];
    }
    int same(int x, int y) {
        return root(x) == root(y);
    }
    vector<vector<int>> groups() {
        vector<vector<int>> a(n);
        for (int i=0; i<n; i++) {
            a[root(i)].push_back(i);
        }
        vector<vector<int>> re;
        for (int i=0; i<n; i++) {
            if (0 < size(a[i])) {
                re.push_back(a[i]);
            }
        }
        return re;
    }
};
