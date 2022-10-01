#include "ideal_cache.hpp"
#include "input.hpp"
#ifdef TEST
#include "test_ideal.hpp"
#endif
int main() {
#ifndef TEST
    input::reqs_t reqs;
    reqs.write_reqs();

    ideal_cache::cache_t<int> ideal(reqs.get_reqs(), reqs.get_cache_size(), reqs.get_reqs_cnt());

    std::cout << ideal.hit_cnt(input::get_page) << std::endl;

    return 0;
#else
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
#endif
}