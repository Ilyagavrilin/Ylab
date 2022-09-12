#include <iostream>
#include "cache_2q.hpp"


struct page_t {
    int id;
};

int main() {
    int b = 3;
    fifo_t<int> a(b);
    std::cout << a.full() << std::endl;
    return 0;
}