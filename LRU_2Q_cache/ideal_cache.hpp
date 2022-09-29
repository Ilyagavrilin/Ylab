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
    struct req_t {
        keyT key;
        unsigned long dst_to_next;
        
        req_t(): key(0), dst_to_next(0) {};
        req_t(keyT key_, unsigned long dst_): key(key_), dst_to_next(dst_) {};
    };
    req_t* reqs;
    size_t size, cur_size, reqs_cnt;
    std::multimap<DstT, page_t> cache;
    using MapIt = typename std::map<DstT, page_t>::iterator;
    std::unordered_map<keyT, MapIt> hash;

    
    
    cache_t(std::list<keyT> &requests, size_t sz, size_t nreqs): cur_size(0), size(sz), reqs_cnt(nreqs) {
        std::map<keyT, unsigned long> elems;
        reqs = new req_t[nreqs];
        unsigned long pos = 0;
        for (keyT req: requests) {
            auto hit = elems.find(req);
            if (hit == elems.end()) {
                elems[req] = pos;
            } else {
                reqs[hit->second].key = hit->first;
                reqs[hit->second].dst_to_next = pos - hit->second;
                elems[req] = pos;
            }

            pos++;
        }
        for (const auto& [key, value] : elems) {
            reqs[value].key = key;
            reqs[value].dst_to_next = ULONG_MAX;
        }

    };
    
    void dump_reqs() const {
        for (int i = 0; i < reqs_cnt; i++) {
            std::cout << "{req: " << reqs[i].key << ", distance:" << reqs[i].dst_to_next << "}; ";
         }
    }
    
    unsigned long count_length(unsigned long distance, unsigned long offset) {
        if (distance == ULONG_MAX) return ULONG_MAX;
        else return distance + offset;
    }

    bool full() const{
        if (cur_size >= size) return true;
        else return false;
    };
    //a<b - true
    bool compare(page_t &a, page_t &b) const{
        return a.dst_to_next < b.dst_to_next;
    };
    
    void dump() const {
        std::cout << "current size: " << cur_size << " ";
        std::cout << "maximum size:" << size << std::endl;
        for (auto elem = cache.begin(); elem != cache.end(); elem++) {
            std::cout << "{ key: " << elem->first << ", elem: " << elem->second.key << "}; ";
        }
        std::cout << std::endl;
    }
    
    bool add_req(unsigned long nreq, req_t cur_req, T(*slow_get_page)(keyT)) {
        auto hit = hash.find(cur_req.key);
        if (hit == hash.end()) {
            if (full()) {
                MapIt to_delete = std::prev(cache.end());
                hash.erase(to_delete->second.key);
                cache.erase(to_delete);
                cur_size--;
            }
            page_t to_add(slow_get_page(cur_req.key), cur_req.key);
            MapIt added = cache.insert(std::pair{count_length(cur_req.dst_to_next, nreq), to_add});
            hash[cur_req.key] = added;
            cur_size++;
            return false;
        } else {
            MapIt hit_map = hit->second;
            page_t hit_page = hit_map->second;
            cache.erase(hit_map);
            MapIt added = cache.insert(std::pair{count_length(cur_req.dst_to_next, nreq), hit_page});
            hash[cur_req.key] = added;
            return true;
        }

        
    }
    int hit_cnt(T(*slow_get_page)(keyT)) {
        int hits = 0;
        for (unsigned long nreq = 0; nreq < reqs_cnt; nreq++) {
            if (add_req(nreq, reqs[nreq], slow_get_page)) {
                hits++;
            }    
#ifdef DEBUG 
            dump();
#endif                
            
        }
        return hits;         
    }

};
}