# OPST
Before compiling and running, please install [sdsl](https://github.com/simongog/sdsl-lite/tree/master).
```bash
g++ -std=c++11 -DNDEBUG Successor.cpp -o Successor -I~/include -L~/lib -lsdsl -ldivsufsort -ldivsufsort64 
./Successor
```
```bash 
g++ -std=c++11 -DNDEBUG Predecessor.cpp -o Predecessor -I~/include -L~/lib -lsdsl -ldivsufsort -ldivsufsort64
./Predecessor
```

We can print the detailed information by setting:
```cpp
#define VERBOSE
```