make
for k in {20..100..20}
do 
    r=16
    while [ $r -le 2048 ]
    do
        inputFileName="ecg_1_to_40.txt_merged$k.txt"
        echo "Processing file: $inputFileName with r=$r via flag_map"
        ./my_program -f $inputFileName -r $r
        r=$(( r * 2 ))
    done
done



