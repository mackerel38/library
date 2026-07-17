#include "structure/dynamicconnectivity.hpp"

int main() {
    poe::dynamicconnectivity dc(4, 6);
    dc.add(0, 0, 1);
    const int connected_before = dc.same(1, 0, 2);
    dc.add(2, 1, 2);
    const int connected_after = dc.same(2, 0, 2);
    const int size = dc.size(3, 1);
    dc.remove(4, 0, 1);
    const int components = dc.count(4);
    const int disconnected = dc.same(5, 0, 2);

    const std::vector<int> answer = dc.solve();
    assert(answer[connected_before] == 0);
    assert(answer[connected_after] == 1);
    assert(answer[size] == 3);
    assert(answer[components] == 3);
    assert(answer[disconnected] == 0);

    poe::dynamicconnectivity multi(2, 5);
    multi.add(0, 0, 1);
    multi.add(1, 1, 0);
    multi.remove(2, 0, 1);
    const int still_connected = multi.same(2, 0, 1);
    multi.remove(3, 0, 1);
    const int finally_removed = multi.same(3, 0, 1);
    const auto result = multi.solve();
    assert(result[still_connected] == 1);
    assert(result[finally_removed] == 0);
}
