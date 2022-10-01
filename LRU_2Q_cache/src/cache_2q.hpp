#pragma once
#include <iostream>
#include <list>
#include <unordered_map>
#include <cassert>


namespace cache_2q {

enum class Qname: int {
    FIFO_IN,
    FIFO_OUT,
    LRU,
    NOT_ALLOC,
};

template <typename T> struct queue_t {
    std::list<T> queue;
    using ListIt = typename std::list<T>::iterator;
    size_t cur_size, size;
    Qname name;
    bool full() const {
        if (cur_size >= size) return true;
        else return false;
    }
    
#ifdef DUMP_QUEUE //don`t work for enum class    
    void dump_queue() {
        std::cout << "queue name: ";
        switch (name){
            case Qname::FIFO_IN: std::cout << "FIFO_IN" << std::endl; break;
            case Qname::FIFO_OUT: std::cout << "FIFO_OUT" << std::endl; break;
            case Qname::LRU: std::cout << "LRU" << std::endl; break;
            default: std::cout << "NOT ALLOCATED" << std::endl; break;
        }
    }
#endif    
    
    void dump(void (*dumper)(T elem)) const {
        
        //dump_queue();
        std::cout << "{ ";
        for (T element:queue) {
            dumper(element);
            std::cout << "; ";
        }
        std::cout << "}" << std::endl;
        std::cout << "current size: " << cur_size << ", max size: " << size << std::endl;
    }

    queue_t() {
        cur_size = 1;
        size = 0;
        name = Qname::NOT_ALLOC;
    
    }

    queue_t(size_t sz, Qname nm) {
        size = sz;
        cur_size = 0;
        name = nm;
    }

    T pop_end() {
        assert(cur_size > 0);
        T removed = queue.back();
        queue.pop_back();
        cur_size--;
        return removed;
    }

    void push_start(T val) {
        assert(!full());

        queue.push_front(val);
        cur_size++;
    }

    void delete_iter(ListIt to_delete) {
        assert(cur_size > 0);
        queue.erase(to_delete);
        cur_size--;
    }

    void transfer_page(ListIt to_transfer) {
        assert(cur_size > 0);
        assert(name == Qname::LRU);

        queue.splice(queue.begin(), queue, to_transfer);
    }

    ListIt get_first() {
        assert(cur_size > 0);
        return queue.begin();
    }

    ListIt get_last() {
        assert(cur_size > 0);
        return std::prev(queue.end());
    }
} ;

template <typename T, typename keyT = int> struct page_t {
    T page;
    keyT key;
    Qname name;
    page_t(T pg, keyT ky, Qname nm) {
        page = pg;
        key = ky;
        name = nm;
    };
};


struct q_size_t {
    size_t fifo_in, fifo_out, lru;
    q_size_t(size_t size) {
        
        fifo_in = size / 4;
        lru = size - fifo_in;
        fifo_out = lru / 2;
        
    }

};

void dump_page(page_t<int> page){
        std::cout << "key: " << page.key << ", name: " << (int)page.name;
    }
void dump_key(int key){
    std::cout << "key: " << key;
}
template <typename T, typename keyT = int> struct cache_t {
    bool fifo_mode;

    size_t size;
    // general queues for pages
    queue_t<page_t<T>> fifo_in, lru;
     // addres only queue (look https://www.vldb.org/conf/1994/P439.PDF page 441 (2Q full version))
    queue_t<keyT> fifo_out;
    
    using ListIt_general = typename std::list<page_t<T>>::iterator;
    std::unordered_map<keyT, ListIt_general> hash_general;
    
    using ListIt_addr = typename std::list<keyT>::iterator;
    std::unordered_map<keyT, ListIt_addr> hash_addr;

    cache_t(size_t sz) {
        assert(sz > 0);
        size = sz;
        if (sz >= 4) {
            q_size_t q_sz(sz);
            fifo_in = queue_t<page_t<T>>(q_sz.fifo_in, Qname::FIFO_IN);
            lru = queue_t<page_t<T>>(q_sz.lru, Qname::LRU);
            fifo_out = queue_t<keyT>(q_sz.fifo_out, Qname::FIFO_OUT);
            fifo_mode = false;
        } else {
            fifo_in = queue_t<page_t<T>>(size, Qname::FIFO_IN);
            fifo_mode = true;
        }
    }
    
    bool add_fifo(keyT req, T(*slow_get_page)(keyT)) {
        auto hit = hash_general.find(req);
        if (hit == hash_general.end()) {
            if (fifo_in.full()) {
                page_t<T> page = fifo_in.pop_end();
                hash_general.erase(page.key);
                page.name = Qname::NOT_ALLOC;
                
            }
            page_t<T> to_add(slow_get_page(req), req, Qname::FIFO_IN);
            fifo_in.push_start(to_add);
            hash_general[req] = fifo_in.get_first();
            return false;
        } else {
            return true;
        }
    }
    
    bool add_req(keyT req, T(*slow_get_page)(keyT)) {

#ifdef DEBUG
        std::cout << "request: " << req << std::endl;
        std::cout << "fifo in:" << std::endl;
        fifo_in.dump(dump_page);
        std::cout << "fifo out:" << std::endl;
        fifo_out.dump(dump_key);
        std::cout << "lru:" << std::endl;
        lru.dump(dump_page);
        std::cout << std::endl;
#endif
        if (fifo_mode) return add_fifo(req, slow_get_page);        
        
        auto hit = hash_general.find(req);
        if (hit == hash_general.end()) {
            //not in fifo_in and lru
            auto hit_addr = hash_addr.find(req);
            if (hit_addr == hash_addr.end()){
                //not in fifo_out adding to fifo_in
                if (fifo_in.full()) {
                    page_t<T> page = fifo_in.pop_end();
                    hash_general.erase(page.key);
                    page.name = Qname::NOT_ALLOC;

                    if (fifo_out.full()) {
                        hash_addr.erase(*(fifo_out.get_last()));
                        fifo_out.pop_end();
                    }
                    fifo_out.push_start(page.key);
                    hash_addr[page.key] = fifo_out.get_first();
                }
                page_t<T> to_add(slow_get_page(req), req, Qname::FIFO_IN);
                fifo_in.push_start(to_add);
                hash_general[req] = fifo_in.get_first();
            } else {
                //in fifo_out, load page then add to lru
                ListIt_addr key = hit_addr->second;
                fifo_out.delete_iter(key);
                hash_addr.erase(*key);

                if (lru.full()){ 
                    keyT deleted_key = (lru.pop_end()).key;
                    hash_general.erase(deleted_key);
                }
                page_t<T> to_add(slow_get_page(req), req, Qname::LRU);
                lru.push_start(to_add);
                hash_general[req] = lru.get_first();
            }
            return false;
        } else {
            //in fifo_in or lru
            //if in fifo_in - do nothing
            ListIt_general page_it = hit->second;
            page_t<T> page = *page_it;
            if (page.name == Qname::LRU) {
                lru.transfer_page(page_it);
            }
            return true;
        }
    }
    
    int hit_cnt(const std::list<keyT> &requests, T(*slow_get_page)(keyT)) {
        int hit = 0;
        for (keyT key: requests) {
            if (add_req(key, slow_get_page)) hit++;
        }
        return hit;
    }

};
}