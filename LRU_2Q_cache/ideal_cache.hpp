#include <list>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <cassert>


template <typename T, typename keyT = int> struct page_t_ {
    T page;
    keyT key;
    int dst_to_next;
    page_t_(T page_, keyT key_, int dst_to_next_) {
        page = page_;
        key =key_;
        dst_to_next = dst_to_next_;
    }
};

template <typename T, typename keyT = int> struct cache_t {
    using page_t = page_t_<T>;
    using ReqIt = typename std::list<keyT>::iterator;
    std::list<keyT> requests;
    std::list<page_t> cache;
    size_t size, cur_size;
    using ListIt = typename std::list<page_t>::iterator;
    std::unordered_map<keyT, ListIt> hash;
    
    cache_t (std::list<keyT> &req, size_t sz) {
        requests.splice(requests.end(), req);
        size = sz;
        cur_size = 0;
    }
    bool full() const {
        if (cur_size == size) return true;
        else return false;
    };
    //a<b - true
    bool compare(page_t &a, page_t &b) const {
        if (b.dst_to_next == -1) {return true;}
        if (a.dst_to_next == -1) {return false;}
        return a.dst_to_next < b.dst_to_next;
    };
    //check comparsion
    ListIt select_pos(page_t &page) {
        for (ListIt ins_pos = cache.end(); ins_pos != cache.begin(); ins_pos--) {
            if (compare(*std::prev(ins_pos), page)){ 
                return ins_pos;
            }
        }
        if (cur_size == 0) return cache.end();
        return cache.begin();
    }
    
    ListIt insert_to_cache(page_t &page) {
        assert(!full());
        auto pos = select_pos(page);
        
        return cache.insert(pos, page);
        
    }

    void cache_step() {
        for (auto page = cache.begin(); page != cache.end(); page++) {
            if ((*page).dst_to_next != -1) (*page).dst_to_next--;
        }
    }
    void transfer_page(ListIt &page, int new_dst) {
        (*page).dst_to_next = new_dst;
        /*auto pos = cache.begin();
        while (pos != cache.end() && compare(*page, *pos)){
            pos++;
        }
        if (pos != cache.end()) pos++;
        */
        auto pos = select_pos(*page);
        cache.splice(pos, cache, page);
    }
    
    int count_length(ReqIt cur_req) {
        ReqIt elem;
        keyT val = *cur_req;
        elem = std::find(++cur_req, requests.end(), val);
        if (elem == requests.end()) return -1;
        else return std::distance(cur_req, elem) + 1;
    }

    bool add_req(ReqIt& cur_req, T(*slow_get_page)(keyT)) {
        auto hit = hash.find(*cur_req);
        cache_step();
        if (hit == hash.end()) {
            if (full()) {
                hash.erase(cache.back().key);
                cache.pop_back();
                cur_size--;
            }
            page_t page(slow_get_page(*cur_req), *cur_req, count_length(cur_req));
            hash[page.key] = insert_to_cache(page);
            cur_size++;
            return false;
            
        }
        auto page_it = hit->second;
        transfer_page(page_it, count_length(cur_req));
        return true;
    }

    void dump() const {
        std::cout << "current size: " << cur_size << ", max size: " << size << std::endl;
        print();
        std::cout << std::endl;
    }
    
    void print() const {
        std::cout << "{ ";
        for (page_t page: cache) {
            std::cout << "val: " << page.key << ", distance: " << page.dst_to_next << "; ";
        }
        std::cout << "} " << std::endl;
    }

    int hit_cnt(T(*slow_get_page)(keyT)) {
        int hits = 0;
        for (auto key = requests.begin(); key != requests.end(); key++) {
            if (add_req(key, slow_get_page)) {
                hits++;
            }
            
        }
        return hits;         
    }
};