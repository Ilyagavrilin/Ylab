#include <list>
#include <cassert>
#include "ideal_cache.hpp"
#include "cache_2q.hpp"

int get_page(int key)
{
    return key;
}

int main()
{
    std::list<int> requests ;
    int cnt;
    size_t cache_sz;
    std::cin >> cache_sz >> cnt;
    assert(std::cin.good());
    for (int i = 0; i < cnt; i++) {
        int a;
        std::cin >> a;
        requests.push_back(a);
    }
    
    ideal_cache::cache_t<int> ideal(requests, cache_sz);
    cache_2q::cache_t<int> cache2q(cache_sz);

    std::cout << "cache 2q: " << cache2q.hit_cnt(requests, get_page) << std::endl;
    std::cout << "ideal cache: " << ideal.hit_cnt(get_page) << std::endl;

    
    return 0;
}
