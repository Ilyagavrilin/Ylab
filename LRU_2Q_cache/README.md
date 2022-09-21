# LRU_2Q and ideal cache comparsion
This project - realization of LRU_2Q algorithm, and comparsion with ideal cache (based on Belady`s algorithm).  
Realization prepared as homework for 2-nd course Ylab course.  
Link to idea of LRU_2Q algorithm: https://www.vldb.org/conf/1994/P439.PDF .  
All tests were on Linux Ubuntu 22.04 LTS .
## Compilation
You can compile programm with command: `make all`, and call it: `./cache`.
## Built-in debug mode
To compile in debug mode: `make debug`, and call it: `./cache`.   
Demonstrates dump of cache`s lists each step (request).
## Input parameters
After call of compilated programm type size of cache, number of requests, then requests. For sizes smaller than 4 will be called other realization of 2q cache, look 2Q cache size problems.
## 2Q cache size problems
In lru 2Q we have 3 queues, so we can not split some small sizes for 3 queues. As a  silution: for small sizes (smaller than 4) we use fifo queue like cache. So, this is not best solution, but it is do not influence on main realization of alghorithm.
