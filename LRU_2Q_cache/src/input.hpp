#pragma once
#include <iostream>
#include <list>
#include <cassert>
namespace input {

int get_page(int key) {
    return key;
}
class reqs_t {
    private:
    size_t reqs_cnt = 0, cache_sz = 0;
    std::list<int> reqs;
    public:
    void write_reqs() {
        std::cin >> cache_sz >> reqs_cnt;
        assert(std::cin.good());
        for (size_t i = 0; i < reqs_cnt; i++) {
            int a;
            std::cin >> a;
            reqs.push_back(a);
        }
    }

    size_t get_reqs_cnt() const {
        return reqs_cnt;
    }

    size_t get_cache_size() const {
        return cache_sz;
    }

    std::list<int> get_reqs() const {
        return reqs;
    }

};

}