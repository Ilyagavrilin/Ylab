#include <list>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <climits>
#include <map>

namespace ideal_cache {
template <typename T, typename keyT = int> struct page_t_ {
    T page;
    keyT key;
    
    page_t_(T page_, keyT key_): key(key_), page(page_) {};
};

template <typename T, typename keyT = int, typename DstT = unsigned long> struct cache_t {
    using page_t = page_t_<T>;
    using ReqIt = typename std::list<keyT>::iterator;
    std::list<keyT> &requests;
    size_t size, cur_size;
    std::multimap<DstT, page_t> cache;
    using MapIt = typename std::multimap<DstT, page_t>::iterator;
    std::unordered_map<keyT, MapIt> hash;

    
    
    cache_t(std::list<keyT> &req, size_t sz): requests(req) {
        size = sz;
        cur_size = 0;
    };
    
    unsigned long count_length(ReqIt cur_req, unsigned long offset) {
        ReqIt elem;
        keyT val = *cur_req;
        elem = std::find(++cur_req, requests.end(), val);
        if (elem == requests.end()) return ULONG_MAX;
        else return std::distance(cur_req, elem) + 1 + offset;
    }

    bool full() const{
        if (cur_size >= size) return true;
        else return false;
    };
    //a<b - true
    bool compare(page_t &a, page_t &b) const{
        return a.dst_to_next < b.dst_to_next;
    };
    
    bool add_req(unsigned long nreq, ReqIt& cur_req, T(*slow_get_page)(keyT)) {
        auto hit = hash.find(*cur_req);
        if (hit == hash.end()) {
            if (full()) {
                MapIt to_delete = std::prev(cache.end());
                hash.erase(to_delete->second.key);
                cache.erase(to_delete);
                cur_size--;
            }
            page_t to_add(slow_get_page(*cur_req), *cur_req);
            MapIt added = cache.insert(std::pair{count_length(cur_req, nreq), to_add});
            hash[*cur_req] = added;
            cur_size++;
            return false;
        } else {
            MapIt hit_map = hit->second;
            page_t hit_page = hit_map->second;
            cache.erase(hit_map);
            MapIt added = cache.insert(std::pair{count_length(cur_req, nreq), hit_page});
            hash[*cur_req] = added;
            cur_size++;
            return true;
        }

        
    }
    int hit_cnt(T(*slow_get_page)(keyT)) {
        int hits = 0, nreq = 0;
        for (auto key = requests.begin(); key != requests.end(); key++) {
            if (add_req(nreq, key, slow_get_page)) {
                hits++;
                
#ifdef DEBUG 
                dump();
#endif                
            }
            nreq++;
            
        }
        return hits;         
    }

};
}