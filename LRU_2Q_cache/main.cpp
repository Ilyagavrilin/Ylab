#include <list>
#include "ideal_cache.hpp"

int get_page(int key)
{
    return key;
}

int main()
{
    std::list<int> req ;
    int cnt;
    size_t cache_sz;
    std::cin >> cnt;
    std::cin >> cache_sz;
    for (int i = 0; i < cnt; i++) {
        int a;
        std::cin >> a;
        req.push_back(a);
    }
    

    cache_t<int> a(req, cache_sz);
    std::cout << a.hit_cnt(get_page) << std::endl;

    return 0;
}