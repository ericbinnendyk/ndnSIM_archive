# install.packages('ggplot2')
library(ggplot2)
# install.packages('scales')
library(scales)

# install.packages('doBy')
library(doBy)

# change this line to plot a different log file
data = read.table("pit-size-r3-no-mitigation.txt", header=T)
#data = read.table("pit-size-r2-expired-pit-mitigation.txt", header=T)
data$Seconds = as.numeric(as.character(factor(data$Seconds)))
print(typeof(data$Entries[299]))
print(data$Entries)
data$Entries = as.numeric(as.character(factor(data$Entries)))

print(data$Entries)

# graph rate on node C2 in Kilobits
g.all <- ggplot(data, aes(x=Seconds, y=Entries)) +
    geom_point()

png("r3-pit-size-no-mitigation.png", width=500, height=250)
print(g.all)
retval <- dev.off()
