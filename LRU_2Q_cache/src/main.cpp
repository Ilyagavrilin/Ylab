#include "ideal_cache.hpp"
#include "cache_2q.hpp"
#include "input.hpp"



int main()
{
    input::reqs_t reqs;
    reqs.write_reqs();
    ideal_cache::cache_t<int> ideal(reqs.get_reqs(), reqs.get_cache_size(), reqs.get_reqs_cnt());
    cache_2q::cache_t<int> cache2q(reqs.get_cache_size());
    
    std::cout << "cache 2q: " << cache2q.hit_cnt(reqs.get_reqs(), input::get_page) << std::endl;
    std::cout << "ideal cache: " << ideal.hit_cnt(input::get_page) << std::endl;
    
    return 0;
}
