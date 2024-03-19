make clean
make
for ((k = 20; k <= 100; k += 20))
do
    inputFileName="ecg_1_to_40.txt_merged$k.txt"
    echo "Processing file: $inputFileName"
    ./my_program -f $inputFileName
done

