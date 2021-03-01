threshold=.001
num=0
seed=1

while [ $num -lt 60 ]
do
    for i in {1..20}
    do
        echo $seed $threshold
        ./waf --run="ifa-mitigation-gini-impurity-roc --seed=$seed --threshold=$threshold" >> "positive_rates_$threshold.txt"
        seed=$(( seed + 1 ))
    done
    threshold=$(echo "$threshold + 0.001" | bc)
    num=$(( num + 1 ))
done
