# Test to try to use Neural Network to predict the most efficient forwarding strategy

# first neural network with keras tutorial
from numpy import loadtxt, mean
from keras.models import Sequential
from keras.layers import Dense

# intended setup of input file:
#interestsoftype1,interestsoftype2,interestsoftype3...
#one line for each unit of time
f = open("nn-input.txt", 'r')
stuff = []
for line in f:
    stuff.append([int(s) for s in line.strip('\n').split(',')])
f.close()

avg_interests = [mean([x[i] for x in stuff]) for i in range(len(stuff[0]))]

# 30 interests each from 4 input locations (in a specific period of time)
# Round trip time of (say) 10 ms
#dataset = [[30] * 4 + [10]]
dataset = np.array([avg_interests])

X = dataset[:,0:4]
y = dataset[:,4]

print("x and y are")
print(X, y)

model = Sequential()
model.add(Dense(12, input_dim=4, activation='relu'))
model.add(Dense(8, activation='relu'))
model.add(Dense(1, activation='sigmoid'))

# compile the keras model
model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])

# fit the keras model on the dataset
model.fit(X, y, epochs=150, batch_size=10)