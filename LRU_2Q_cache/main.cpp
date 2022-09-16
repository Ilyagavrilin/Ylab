#include <list>
#include "ideal_cache.hpp"

int get_page(int key) {
    return key;
}

int main()
{
    std::list<int> req = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2};
    size_t cache_sz = 3;
    cache_t<int> a(req, cache_sz);
    std::cout << a.hit_cnt(get_page) << std::endl;
    
    return 0;
}