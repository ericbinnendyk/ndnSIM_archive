# install.packages('ggplot2')
library(ggplot2)
# install.packages('scales')
library(scales)

# install.packages('doBy')
library(doBy)

data = read.table("results/tmp/topo-small-binary-tree-eric.txt", header=T)
data$Node = factor(data$Node)
data$FaceId <- factor(data$FaceId)
data$Kilobits <- data$Kilobytes * 8
data$Type = factor(data$Type)

# exlude irrelevant types
data = subset(data, Type %in% "InInterests")
data = subset(data, Node %in% "NodeR1")
print(data)

# combine stats from all faces
data.combined = summaryBy(. ~ Time + Node + Type, data=data, FUN=sum)

# graph rate on node C2 in Kilobits
g.all <- ggplot(data.combined) +
  geom_point(aes (x=Time, y=Packets.sum), size=1) +
  ylab("Rate [packets/s]")

png("graphs/r2-interest-rate-test.png", width=500, height=250)
print(g.all)
retval <- dev.off()
