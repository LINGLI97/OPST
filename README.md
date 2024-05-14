# OPST
Before compiling and running, please install [sdsl](https://github.com/simongog/sdsl-lite/tree/master).
```bash
./pre-install.sh
```


## Compile and run

```
make all
```

Consider w = 1 2 4 4 2 5 5 1 as the input  in the sample.txt, we can run the methods as below:
```bash 
#index construction
./OPSTConstruction -f sample.txt

#mining maximal OP patterns
./OPSTMaximal -f sample.txt -t 2
./BAMP -f sample.txt -t 2

#mining closed OP patterns
./OPSTClosed -f sample.txt -t 2
./BACP -f sample.txt -t 2
```

The `-f` option determines the file path;   
The `-t` option determines the value of minimal support, tau > 1. 

For MOPP, please refer to [this github](https://github.com/wuc567/Pattern-Mining/tree/master/OPP-Miner).

## Print Out Information

--------------------------------------------Information Board--------------------------------------------------------  
Enabling baseline - Maximal patterns mining:   
Processing sample.txt  
Tau is set as 2.  
The max length of patterns: 3  
The input size: 8  
Runtime used for mining 2-maximal order-preserving 2-frequent patterns: 9.8e-05 s.  
The number of found maximal patterns is 2  

--------------------------------------------Information Board--------------------------------------------------------  
Enabling  baseline - Closed patterns mining:   
Processing sample.txt  
Tau is set as 2.  
The max length of patterns: 3  
The input size: 8  
Runtime used for mining 2-closed order-preserving 2-frequent patterns: 0.000126 s.  
The number of found closed patterns is 4  

--------------------------------------------Information Board--------------------------------------------------------  
Enabling OPST maximal method  
Processing sample.txt  
Tau is set as 2.  
The input size: 8  
Terminate_label (2 * n + 1) $ = 17  
Runtime only for OPST construction  = 9.7e-05 s.  
Runtime only used for mining 2-maximal order-preserving 2-frequent patterns: 1.9e-05 s.  
The number of found maximal patterns is 2  
Total runtime for OPST construction and Maximal patterns mining : 0.000116 s.  
The max length of patterns:3  

--------------------------------------------Information Board--------------------------------------------------------  
Enabling OPST closed method  
Processing sample.txt  
Tau is set as 2.  
The input size: 8  
Terminate_label (2 * n + 1) $ = 17  
Runtime only for OPST construction  = 0.000157 s.  
Runtime only used for mining  2-closed time1 order-preserving 2-frequent patterns: 2e-05 s.  
Runtime only used for mining 2-closed order-preserving 2-frequent patterns: 3.4e-05 s.  
The number of found closed patterns is 4  
Total runtime for OPST construction and Closed patterns mining : 0.000191 s.  
The max length of patterns:3  


## Datasets
We uploaded the datasets to [Google Drive]().
