#pragma once
#include <bits/stdc++.h>
#include "geometry/point.hpp"

namespace poe {

/// affine<T>: 2次元アフィン変換。left*rightはrightを適用してからleftを適用する。
template<class T>
struct affine {
    T xx{1}, xy{}, x0{};
    T yx{}, yy{1}, y0{};

    /// O(1)。transform(p): 点pを変換する。
    point<T> operator()(const point<T>& value) const {
        return {xx * value.x + xy * value.y + x0, yx * value.x + yy * value.y + y0};
    }

    /// O(1)。identity(): 恒等変換を返す。
    static affine identity() { return {}; }

    /// O(1)。translate(dx,dy): 平行移動を返す。
    static affine translate(T dx, T dy) { return {1, 0, dx, 0, 1, dy}; }

    /// O(1)。rotate_clockwise(): 原点中心の時計回り90度回転を返す。
    static affine rotate_clockwise() { return {0, 1, 0, -1, 0, 0}; }

    /// O(1)。rotate_counterclockwise(): 原点中心の反時計回り90度回転を返す。
    static affine rotate_counterclockwise() { return {0, -1, 0, 1, 0, 0}; }

    /// O(1)。reflect_x(x): 直線X=xに関する鏡映を返す。
    static affine reflect_x(T x) { return {-1, 0, 2 * x, 0, 1, 0}; }

    /// O(1)。reflect_y(y): 直線Y=yに関する鏡映を返す。
    static affine reflect_y(T y) { return {1, 0, 0, 0, -1, 2 * y}; }

    /// O(1)。left*right: rightの後にleftを適用する合成変換を返す。
    friend affine operator*(const affine& left, const affine& right) {
        return {
            left.xx * right.xx + left.xy * right.yx,
            left.xx * right.xy + left.xy * right.yy,
            left.xx * right.x0 + left.xy * right.y0 + left.x0,
            left.yx * right.xx + left.yy * right.yx,
            left.yx * right.xy + left.yy * right.yy,
            left.yx * right.x0 + left.yy * right.y0 + left.y0
        };
    }
};

}
