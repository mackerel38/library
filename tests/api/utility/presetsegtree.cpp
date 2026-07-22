#include "utility/presetsegtree.hpp"

int main() {
    const std::vector<long long> values = {1, 2, 3, 4};
    poe::point_add_range_sum<long long> point_add(values);
    point_add.add(0, 2);
    point_add.set(1, 5);
    assert(point_add.sum() == 15 && point_add[0] == 3);

    poe::point_affine_range_sum<long long> point_affine(values);
    point_affine.affine(1, 2, 3);
    assert(point_affine.sum(0, 2) == 8);

    poe::point_flip_range_sum point_flip({0, 1, 1});
    point_flip.flip(0);
    assert(point_flip.sum() == 3);

    poe::point_set_range_min<long long> point_min(values);
    poe::point_set_range_max<long long> point_max(values);
    poe::point_set_range_gcd<long long> point_gcd({6, 9, 15});
    assert(point_min.min(1, 4) == 2);
    assert(point_max.max(1, 4) == 4);
    assert(point_gcd.gcd() == 3);

    poe::point_set_prefix_sum_max<long long> prefix({-2, 5, -4, 3});
    assert(prefix.sum(3) == -1 && prefix.max_prefix() == 3);
    assert(prefix.first_prefix_at_least(0) == 1);
    prefix.set(1, 1);
    assert(prefix.first_prefix_at_least(0) == 4);
    prefix.add(0, 4);
    assert(prefix[0] == 2 && prefix.first_prefix_at_least(0) == 0);

    using composite = poe::point_set_range_composite<long long>;
    composite functions(std::vector<composite::affine>{{2, 1}, {3, 4}});
    assert(functions.apply(0, 2, 5) == 37);

    poe::range_add_range_sum<long long> add_sum(values);
    add_sum.add(1, 4, 2);
    assert(add_sum.sum() == 16 && add_sum[2] == 5);

    poe::range_add_range_sum_square<long long> add_square(values);
    add_square.add(1, 4, 2);
    assert(add_square.sum() == 16 && add_square.square_sum() == 78);
    add_square.set(2, -3);
    assert(add_square[2] == -3 && add_square.square_sum(1, 4) == 61);

    poe::range_set_range_sum<long long> set_sum(values);
    set_sum.set(1, 3, 7);
    assert(set_sum.sum() == 19);

    poe::range_affine_range_sum<long long> affine_sum(values);
    affine_sum.affine(0, 3, 2, 1);
    assert(affine_sum.sum() == 19);

    poe::range_add_range_min<long long> add_min(values);
    poe::range_add_range_max<long long> add_max(values);
    add_min.add(0, 2, -5);
    add_max.add(2, 4, 5);
    assert(add_min.min() == -4 && add_max.max() == 9);

    poe::range_subtract_clamp_zero_sum<long long> consume({2, 6, 4, 5});
    assert(consume.subtract(0, 4, 3) == 11);
    assert(consume[0] == 0 && consume[1] == 3 && !consume.active(0));
    consume.set(0, 7);
    assert(consume.subtract(0, 2, 2) == 4 && consume[0] == 5);

    poe::range_set_range_min<long long> set_min(values);
    poe::range_set_range_max<long long> set_max(values);
    poe::range_set_range_gcd<long long> set_gcd({6, 9, 15});
    set_min.set(0, 2, 8);
    set_max.set(2, 4, -1);
    set_gcd.set(0, 2, 12);
    assert(set_min.min() == 3);
    assert(set_max.max() == 2);
    assert(set_gcd.gcd() == 3);

    poe::range_add_range_gcd<long long> add_gcd({6, 10, 14});
    add_gcd.add(0, 2, 2);
    assert(add_gcd.gcd() == 2);

    poe::range_add_point_get<long long> add_get(values);
    poe::range_set_point_get<long long> set_get(values);
    poe::range_affine_point_get<long long> affine_get(values);
    add_get.add(0, 2, 4);
    set_get.set(1, 4, 9);
    affine_get.affine(2, 4, 3, 1);
    assert(add_get[1] == 6 && set_get[2] == 9 && affine_get[3] == 13);

    poe::range_flip_range_sum flip_sum({0, 1, 0, 1});
    flip_sum.flip(1, 4);
    assert(flip_sum.sum() == 1);

    poe::range_flip_range_inversion inversion({1, 0, 1, 0});
    assert(inversion.inversion(0, 4) == 3);
    inversion.flip(0, 4);
    assert(inversion.inversion(0, 4) == 1);

    poe::range_active_add_range_clear_flip_max<long long> active(4);
    active.add(0, 3, 5);
    active.clear_flip(1, 4);
    active.add(0, 4, 2);
    assert(active.max() == 7 && active[0] == 7 && active[1] == 0);
    assert(active.active(0) && !active.active(1));
    active.set(1, -3);
    active.deactivate(0);
    assert(active.max(0, 2) == 0);

    poe::range_divide_set_range_sum<long long> divide_sum({10, 3, 5, 20});
    divide_sum.divide(1, 4, 3);
    assert(divide_sum.sum() == 18 && divide_sum[2] == 1);
    divide_sum.set(0, 3, 8);
    divide_sum.set(1, 5);
    assert(divide_sum.sum(0, 4) == 27 && divide_sum[1] == 5);

    poe::range_chmin_chmax_add_range_sum<long long> beats(values);
    beats.chmin(0, 4, 3);
    beats.chmax(0, 2, 2);
    beats.add(1, 4, 5);
    beats.set(2, -1);
    assert(beats.sum() == 16 && beats.sum(1, 3) == 6 && beats[2] == -1);
}
