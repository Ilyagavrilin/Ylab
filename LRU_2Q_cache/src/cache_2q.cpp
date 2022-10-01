#include "cache_2q.hpp"
#include "input.hpp"
#ifdef TEST
#include "test_2q.hpp"
#endif

int main() {
#ifndef TEST    
    input::reqs_t reqs;
    reqs.write_reqs();

    cache_2q::cache_t<int> cache2q(reqs.get_cache_size());

    std::cout << cache2q.hit_cnt(reqs.get_reqs(), input::get_page) << std::endl;

    return 0;
#else
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
#endif
}