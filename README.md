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



## Executable files 

Without unordered_dense map and optimization flags (-msse3 -O3 -fomit-frame-pointer -funroll-loops):  
```bash 
./my_program_NA      
```
Only with optimization flags:   
```bash 
./my_program_flag 
```
Only with unordered_dense map:  
```bash 
./my_program_map
```
With unordered_dense map and optimization flags:  
```bash 
./my_program_map_flag 
```

You can easily specify parameters, for example:

```bash 
./my_program_NA -f ecg_1_to_40.txt_merged20.txt -r 20 -s 10
```

-r specifies  "the range of (a,b) determines calling the naive or wavelet tree method", the default value is 10.
-s specifies  "the time relations between size and sigma determines calling the naive or wavelet tree method", the  default value is 5.

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
-VISUALIZATION
```
and then 
```bash 
dot -Tpdf pic_nosufL -o suffix_tree_nosuf.pdf  
dot -Tpdf pic_sufL -o suffix_tree_suf.pdf
```



Using unordered_dense from [here](https://github.com/martinus/unordered_dense/tree/main) by setting the following flag. Otherwise, it will utilize std::unordered_map.
```cpp
-DUNORDERED_DENSE
```






The time curve with increasing the file size from 1MB to 20MB:

![time](./timevsSize.jpg)

