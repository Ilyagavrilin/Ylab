#include <list>
#include "ideal_cache.hpp"

int get_page(int key) {
    return key;
}

int main()
{
    std::list<int> req;
    for (int i = 0; i < 100; i++) {
        req.push_back(std::rand() % 100);
    }
    size_t cache_sz = 5;
    cache_t<int> a(req, cache_sz);
    std::cout << a.hit_cnt(get_page) << std::endl;
    
    return 0;
}