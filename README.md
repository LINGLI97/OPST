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

## compile and run
After installing sdsl, you may need to change the include and lib paths in Makefile to your <span style="color: red;">OWN</span> paths. 
```bash 
./run.sh
```

It will run on ecg_1_to_40.txt_merged20.txt ~ ecg_1_to_40.txt_merged100.txt, correpsonding to 20 to 100 MB files. 




## Tips
We can print the detailed information by setting:
```cpp
-DVERBOSE
```
We do some safe checks by setting:
```cpp
-DCHECK
```

We can visulize the suffix tree via Graphviz by setting:
```cpp
-DINT2PS
```
and then 
```bash 
dot -Tpdf pic_nosufL -o suffix_tree_nosuf.pdf  
dot -Tpdf pic_sufL -o suffix_tree_suf.pdf
```



Using unordered_dense from [here](https://github.com/martinus/unordered_dense/tree/main) by setting the following flag. Otherwise, it will utilize the standard unordered map.
```cpp
-DUNORDERED_DENSE
```






The time curve with increasing the file size from 1MB to 20MB:

![time](./timevsSize.jpg)

