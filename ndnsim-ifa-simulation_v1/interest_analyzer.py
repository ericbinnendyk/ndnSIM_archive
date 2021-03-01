#!/usr/bin/python3

# list of collected distributions for each set of 20 seconds
distributions = []
# list of Gini impurities for each distribution
impurities = []
freqs = {}

sample_num = 0

def gini_impurity(item_chances):
    value = 1.0
    for item in item_chances:
        value -= item[1]**2
    return value

input_line = input()
while input_line != '':
    # collect interests into a separate bin every 20 seconds of simulation
    # analyze the Gini impurity of that bin
    if input_line.startswith('+'):
        num_secs = float(input_line.split(' ')[0].strip('+').strip('s'))
        new_sample_num = int(num_secs / 20)
        if new_sample_num > sample_num:
            # analyze distribution of interests
            items = list(freqs.items())
            items.sort(key=(lambda x: x[1]), reverse=True)

            # find probability distribution of each interest
            item_chances = []
            total_requests = sum([x[1] for x in items])
            for item in items:
                item_chances.append((item[0], item[1]/total_requests))

            impurity = gini_impurity(item_chances)
            print(impurity)
            if len(impurities) > 0:
                a_of_impurities = (impurity - impurities[-1])/impurities[-1]
                if a_of_impurities > 0.05:
                    print("There may have been an attack!")
            distributions.append(item_chances)
            impurities.append(impurity)

            # start new frequency distribution
            freqs = {}
            sample_num = new_sample_num

    # collect new interest for distribution
    if 'receiving interest' in input_line:
        data_name = input_line.split(' ')[-1]
        if data_name in freqs:
            freqs[data_name] += 1
        else:
            freqs[data_name] = 1

    try:
        input_line = input()
    except:
        break
