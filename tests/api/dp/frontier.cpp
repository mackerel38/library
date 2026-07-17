#include <cassert>

#include "dp/frontier.hpp"
#include "dp/profile.hpp"
#include "recipe/connectivityfrontier.hpp"

int main() {
    {
        auto state = poe::connectivityprofile::from_mask(3, 0b101, 0);
        assert(state.is_marked(0));
        assert(!state.is_marked(2));
        state = state.advance(0b111);
        assert(state.is_marked(0));
        assert(state.is_marked(1));
        assert(state.is_marked(2));
    }
    {
        poe::frontierdp<int, long long> dp;
        dp.add(1, 2);
        dp.add(1, 3);
        dp.step([](int state, long long ways, auto& next) {
            next.add(state + 1, ways);
            next.add(state + 1, ways);
        });
        assert(dp.size() == 1);
        assert(dp.sum([](int state) { return state == 2; }) == 10);
    }
    {
        poe::connectivityfrontierdp<long long> dp(2, 0);
        dp.advance_all();
        dp.advance_all();
        assert(dp.sum_marked(1) == 8);
    }
}
