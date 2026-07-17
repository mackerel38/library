#include "structure/prefixsum.hpp"

int main() {
    poe::differencearray<int> difference(5);
    difference.add(1, 4, 3);
    difference.add(2, 5, -1);
    difference.build();
    assert(difference.values() == std::vector<int>({0, 3, 2, 2, -1}));
    difference.add(0, 2, 4);
    difference.build();
    assert(difference[0] == 4 && difference[1] == 7 && difference[2] == 2);

    poe::differencearray2d<int> difference2d(3, 4);
    difference2d.add(0, 1, 2, 4, 3);
    difference2d.add(1, 0, 3, 2, -1);
    difference2d.build();
    assert(difference2d(0, 1) == 3);
    assert(difference2d(1, 1) == 2);
    assert(difference2d(2, 3) == 0);
    difference2d.add(2, 3, 3, 4, 7);
    difference2d.build();
    assert(difference2d(2, 3) == 7);

    poe::prefixsum<long long> sums({2, 5, -1, 4});
    assert(sums.size() == 4);
    assert(sums[2] == -1);
    assert(sums.sum(0) == 0);
    assert(sums.sum(3) == 6);
    assert(sums.sum(1, 4) == 8);
    assert(sums.sum() == 10);

    poe::circularprefixsum<long long> circular({3, 1, 4, 5});
    assert(circular.sum(0, 3) == 8);
    circular.rotate(1);
    assert(circular[0] == 1 && circular.sum(1, 3) == 9);
    circular.rotate(-2);
    assert(circular.offset() == 3 && circular[0] == 5);

    poe::prefixsum<long long> empty({});
    assert(empty.size() == 0 && empty.sum() == 0);

    poe::subarrayprefixsum<long long> subarrays({2, 1, 3, 3, 1});
    assert(subarrays.size() == 5 && subarrays[2] == 3);
    assert(subarrays.sum(1, 4) == 24);
    assert(subarrays.sum() == 74);
    assert(subarrays.sum(3, 3) == 0);

    poe::prefixsum2d<long long> grid({{1, 2, 3}, {4, 5, 6}});
    assert(grid.height() == 2 && grid.width() == 3);
    assert(grid(1, 2) == 6);
    assert(grid.sum(2, 2) == 12);
    assert(grid.sum(0, 1, 2, 3) == 16);
    assert(grid.sum() == 21);

    poe::multidimensionalprefixsum<long long> cube(
        {2, 2, 2}, {1, 2, 3, 4, 5, 6, 7, 8});
    assert(cube.sum({0, 0, 0}, {2, 2, 2}) == 36);
    assert(cube.sum({1, 0, 1}, {2, 2, 2}) == 14);
    assert(cube(std::vector<int>{1, 1, 0}) == 7 && cube[7] == 8);
}
