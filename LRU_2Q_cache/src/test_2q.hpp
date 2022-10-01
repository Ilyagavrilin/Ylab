#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <list>
#include <cstdlib>
#include "cache_2q.hpp"

int get_page(int val) {
    return val;
}

struct q2_test : public testing::Test {
    cache_2q::cache_t<int> *cache;
    int nreqs = 10;
    int cache_sz = 6;

    void SetUp() { 
        
        std::list<int> reqs;
        for (int i = 0; i < nreqs; i++) {
            reqs.push_back(i * 2);
        }
        cache = new cache_2q::cache_t<int>(cache_sz); 
    }
    void TearDown() { delete cache; }
};

TEST_F(q2_test, init_test) {
    ASSERT_EQ(cache->size, cache_sz);
    ASSERT_EQ(cache->fifo_mode, false);
}

TEST_F(q2_test, full_test) {
    ASSERT_EQ(cache->fifo_in.full(), false);
    ASSERT_EQ(cache->fifo_out.full(), false);
    ASSERT_EQ(cache->lru.full(), false);
    
    cache->fifo_in.cur_size = 10;
    ASSERT_EQ(cache->fifo_in.full(), true);
    cache->fifo_in.cur_size = 0;
}

TEST_F(q2_test, addition_test) {
    bool res = cache->add_fifo(3, get_page);
    EXPECT_EQ(res, false);
    ASSERT_EQ(cache->fifo_in.cur_size, 1);
}



TEST(e2e_test, test_fifo_mode) {
    int cache_sz = 3, ans = 3;
    std::list<int> reqs = {1, 2, 3, 1, 2, 3};
    cache_2q::cache_t<int> cache(cache_sz);
    int res = cache.hit_cnt(reqs, get_page);
    ASSERT_EQ(res, ans);
}

TEST(e2e_test, test_m_reqs) {
    int cache_sz = 1000, nreqs = 1000000;
    std::list<int> reqs;
    for (int i = 0; i < nreqs; i++) {
        reqs.push_back(std::rand()%cache_sz);
    }
    cache_2q::cache_t<int> cache(cache_sz);
    int res = cache.hit_cnt(reqs, get_page);
    EXPECT_GT(res, cache_sz);
}