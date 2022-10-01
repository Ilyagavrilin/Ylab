#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <list>
#include <cstdlib>
#include "ideal_cache.hpp"

int get_page(int val) {
    return val;
}

struct ideal_test : public testing::Test {
    ideal_cache::cache_t<int> *cache;
    int nreqs = 10;
    int cache_sz = 3;

    void SetUp() { 
        
        std::list<int> reqs;
        for (int i = 0; i < nreqs; i++) {
            reqs.push_back(i * 2);
        }
        cache = new ideal_cache::cache_t<int>(reqs, cache_sz, nreqs); 
    }
    void TearDown() { delete cache; }
};

TEST_F(ideal_test, init_test) {
    ASSERT_EQ(cache->size, cache_sz);
    ASSERT_EQ(cache->cur_size, 0);
    ASSERT_EQ(cache->reqs_cnt, nreqs);
}

TEST_F(ideal_test, full_test) {
    ASSERT_EQ(cache->full(), false);
    
    cache->cur_size = cache_sz;
    ASSERT_EQ(cache->full(), true);
    
    cache->cur_size = cache_sz + 1;
    EXPECT_EQ(cache->full(), true);
}

TEST_F(ideal_test, addition_test) {
    bool res = cache->add_req(0, cache->reqs[0], get_page);
    EXPECT_EQ(res, false);
    ASSERT_EQ(cache->cur_size, 0);
}

TEST_F(ideal_test, length_test) {
    unsigned long length = cache->count_length(ULONG_MAX, 10);
    ASSERT_EQ(length, ULONG_MAX);
}

TEST(e2e_test, test_hits) {
    int cache_sz = 4, nreqs = 12, ans = 7;
    std::list<int> reqs = {1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4};
    ideal_cache::cache_t<int> cache(reqs, cache_sz, nreqs);
    int res = cache.hit_cnt(get_page);
    ASSERT_EQ(res, ans);
}

TEST(e2e_test, test_m_reqs) {
    int cache_sz = 1000, nreqs = 1000000;
    std::list<int> reqs;
    for (int i = 0; i < nreqs; i++) {
        reqs.push_back(std::rand()%cache_sz);
    }
    ideal_cache::cache_t<int> cache(reqs, cache_sz, nreqs);
    int res = cache.hit_cnt(get_page);
    EXPECT_GT(res, cache_sz);
}