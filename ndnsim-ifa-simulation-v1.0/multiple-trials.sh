threshold=.0005
num=0
seed=1

while [ $num -lt 40 ]
do
    for i in {1..50}
    do
        echo $seed $threshold
        ./waf --run="ifa-mitigation-gini-impurity-roc --seed=$seed --threshold=$threshold" >> "positive_rates_2sec_$threshold.txt"
        seed=$(( seed + 1 ))
    done
    threshold=$(echo "$threshold + 0.0005" | bc)
    num=$(( num + 1 ))
done
