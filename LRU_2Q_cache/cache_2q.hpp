#pragma once

#include <iostream>
#include <list>
#include <unordered_map>
#include <cassert>


enum  q_name {
    FIFO_IN, 
    FIFO_OUT, 
    LRU
};

struct cache_sz_t {
        size_t fifo_in, fifo_out, lru, summary;
};

template <typename T> struct caching_t {
    T page;
    q_name name;
};

template <typename T> struct fifo_t {
    std::list<T> queue;
    size_t size, cur_size;
    
    bool full() const {
        if (size == cur_size) return true;
        else return false;
    };
    
    fifo_t(size_t sz) {
        size = sz;
        cur_size = 0;
    };
    
    T add(T page) {
        if (full()) {
        T del_page;
        del_page = queue.back();
        queue.pop_back();
        queue.push_front(page);
        return del_page;
        } else{
        queue.push_front(page);
        cur_size++;
        return page;
        }
    };
    
    template <typename F> void extract(F page) {
        queue.remove(page);
    };
};

template <typename T> struct lru_t {
    std::list<T> queue;
    size_t size, cur_size;
    
    bool full() const {
        if (size == cur_size) return true;
        else return false;
    };
    
    fifo_t(size_t sz) {
        size = sz;
        cur_size = 0;
    };
    
    T add(T page) {
        if (full()) {
        T del_page;
        del_page = queue.back();
        queue.pop_back();
        queue.push_front(page);
        return del_page;
        } else{
        queue.push_front(page);
        cur_size++;
        return page;
        }
    };
    
    template <typename F> void access(F page) {
        if (page != queue.begin()) {
            queue.remove(page);
            queue.push_front(page);
        }
    };
};


//require: need a id field in F

inline cache_sz_t size_dstrb(size_t size) {
    cache_sz_t sz;
    sz.fifo_in = 3;
    return sz;
}

template <typename T, typename keyT = int> struct cache_t {
    cache_sz_t size;
    // general queues for pages
    fifo_t<caching_t<T>> *in;
    lru_t<caching_t<T>> *lru;
     // addres only queue (look https://www.vldb.org/conf/1994/P439.PDF page 441 (2Q full version))
    fifo_t<caching_t<keyT>> *out;
    using ListIt_general = typename std::list<caching_t<T>>::iterator;
    std::unordered_map<keyT, ListIt_general> hash_general;
    using ListIt_addr = typename std::list<caching_t<keyT>>::iterator;
    std::unordered_map<keyT, ListIt_addr> hash_addr;
    cache_t(size_t sz) {
        assert(sz > 0);
        cache_sz_t size = size_dstrb(sz);
        fifo_in = new queue_t<caching_t<T>> (size.fifo_in);
        lru = new queue_t<caching_t<T>> (size.lru);
        fifo_out = new queue_t<caching_t<keyT>> (size.fifo_out);
    };
    template <typename F> bool update_cache(keyT key, F slow_get_page);
};