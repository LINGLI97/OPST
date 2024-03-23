# OPST
Before compiling and running, please install [sdsl](https://github.com/simongog/sdsl-lite/tree/master).
```bash
./pre-install.sh
```


## Compile and run

```bash 
./run.sh
```

It will run on ecg_1_to_40.txt_merged20.txt ~ ecg_1_to_40.txt_merged100.txt, correpsonding to 20 to 100 MB files. 



You can easily specify parameters, for example:

```bash 
./my_program -f ecg_1_to_40.txt_merged20.txt -r 256 -t 2
```
-f specifies the file path;
-r specifies  "the range of (a,b) determines calling the naive or wavelet tree method", the default value is 512;
-t specifies the value of minimal support, tau > 1.

## Datasets
We uploaded the datasets to [Google Drive](https://drive.google.com/file/d/1eAJr_UHhCpWPiZ_a-rC9WOwWO3Rf3txp/view).


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

