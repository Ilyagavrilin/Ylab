# LRU_2Q and ideal cache comparsion
This project - realization of LRU_2Q algorithm, and comparsion with ideal cache (based on Belady`s algorithm).  
Realization prepared as homework for 2-nd course Ylab course.  
Link to idea of LRU_2Q algorithm: https://www.vldb.org/conf/1994/P439.PDF .  
All tests were on Linux Ubuntu 22.04 LTS .
## Build and execution
To build program type `make`, then will be created `executables\`, there you can call `./cache_2q.out` or `./cache_ideal.out`.  
There also `make debug` - make similar binaries, but with debugging info for each step.  
Mode `make test` will build similar executables, but they will run integrated test. (Warning: the gtest lib should be installed in standart libraries path)  
Finally, `make cmp_cache` will build `main.out` thats combines 2q and ideal cache.  

## Input parameters
After call of compilated programm type size of cache, number of requests, then requests. For sizes smaller than 4 will be called other realization of 2q cache, look 2Q cache size problems.
## 2Q cache size problems
In lru 2Q we have 3 queues, so we can not split some small sizes for 3 queues. As a  silution: for small sizes (smaller than 4) we use fifo queue like cache. So, this is not best solution, but it is do not influence on main realization of alghorithm.
