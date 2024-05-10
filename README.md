# OPST
Before compiling and running, please install [sdsl](https://github.com/simongog/sdsl-lite/tree/master).
```bash
./pre-install.sh
```


## Compile and run

```bash 
./run.sh
```




You can easily specify parameters, for example:

```bash 
./my_program --filepath=ecg_1_to_40.txt_discretized --OPST=1 --MaximalQuadratic=1 --ClosedQuadratic=1 --MaximalCubic=1 --ClosedCubic=1 --tau=2 --printPattern=0
```

The `-f` /`--filepath` option determines the file path;   
The `--OPST` option determines whether to enable maximal and closed patterns mining of OPST; the defalut value is true;  
The `--MaximalQuadratic` option determines whether to enable maximal patterns mining of the quadratic baseline; the defalut value is false;  
The `--ClosedQuadratic` option determines whether to enable closed patterns mining of the quadratic baseline; the defalut value is false;  
The `--MaximalCubic` option determines whether to enable maximal patterns mining of the cubic baseline; the defalut value is false;  
The `--ClosedCubic` option determines whether to enable closed patterns mining of the cubic baseline; the defalut value is false;  

The `-t`/`--tau` option determines the value of minimal support, tau > 1. The default value is 512;  
The `-p`/`--printPattern` option determines whether to print the all found maximal patterns. The default value is false.



## Print Out Information

Suffix tree is constructed based on w = 12442551   
--------------------------------------------Information Board--------------------------------------------------------  
Sigma of input = 4  
n = 8  
Terminate_label $ = 91  
The number of middle_implicit_ma x appeared: 1  
In succinct log mode.  
Using std::unordered_map  
Not using safe check  
Tau is set as 2.  
If the range of LastCode input (a, b) , namely b - a < 512, it utilizes the naive way to compute (p(w), s(w)).  
Runtime for wavelet tree construction = 0.009 s.  
Runtime for suffix tree construction = 0 s.  
Total runtime for wavelet tree and suffix tree construction = 0.009 s.  
Runtime used for find 2-maximal order-preserving 2-frequent patterns: 0 s.  
The number of found patterns is 2  
Pattern found at interval: [1, 3]  
Pattern found at interval: [2, 4]  







## Datasets
We uploaded the datasets to [Google Drive]().


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

