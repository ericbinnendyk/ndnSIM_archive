library(tcltk)
options(scipen=10);
options(digits=4);

false_rates = c()
true_rates = c()

threshold = 0.001;
while (threshold < 0.061) {
    print("threshold is");
    print(threshold);
    print(threshold < 0.0014);
    print("filename is");
    print(paste("positive_rates_", substr(format(threshold, digits=3, nsmall=3), 2, 1000000L), ".txt", sep=""));
    data <- scan(file=paste("positive_rates_", substr(format(threshold, digits=3, nsmall=3), 2, 1000000L), ".txt", sep=""), what=character(), sep='\n')
    #data <- scan(file="positive_rates.txt", what=character(), sep='\n')

    true_positives = 0
    true_negatives = 0
    false_positives = 0
    false_negatives = 0

    for (line in data) {
        if (line == 'true positive')
            true_positives = true_positives + 1
        if (line == 'true negative')
            true_negatives = true_negatives + 1
        if (line == 'false positive')
            false_positives = false_positives + 1
        if (line == 'false negative')
            false_negatives = false_negatives + 1
    }

    false_positive_rate = false_positives / (true_negatives + false_positives)
    true_positive_rate = true_positives / (true_positives + false_negatives)

    false_rates = append(false_rates, c(false_positive_rate))
    true_rates = append(true_rates, c(true_positive_rate))
    threshold = threshold + 0.001
}
print(false_rates)
print(true_rates)

X11()
plot(false_rates, true_rates, xlim=c(0, 1), ylim=c(0, 1))

prompt <- "hit spacebar to close plots"
capture <- tk_messageBox(message = prompt)
