import random

route101 = [
3, 1, 3, 3, 3, 35, 3, 3, 3, 3, 34, 3, 3, 19, 12, 12, 20, 3, 3, 3, 33
, 3, 19, 12, 12, 12, 12, 20, 3, 3, 3, 18, 34, 3, 35, 3, 32, 3, 17, 3, 3, 50, 52, 52, 52, 54, 52, 52, 53
, 52, 52, 63, 64, 64, 64, 64, 65, 52, 52, 52, 52, 54, 52, 52, 52, 51, 3, 3, 3, 8, 3, 2, 3, 3, 18, 19, 12
, 20, 3, 3, 34, 39, 3, 3, 40, 3, 42, 3, 32, 3, 17, 3, 3, 3, 3, 15, 3, 3, 48, 3, 23, 24, 18, 3, 3, 18, 3
, 50, 52, 54, 53, 52, 54, 54, 52, 63, 64, 65, 52, 52, 52, 51, 3, 9, 80, 3, 3, 38, 3, 23, 16, 16, 24, 19
, 12, 20, 3, 3, 3, 3, 15, 3, 3, 3, 49, 50, 52, 52, 52, 53, 52, 63, 65, 52, 51, 3, 18, 3, 3, 3, 7, 3, 2, 3
, 3, 3, 3, 3, 3, 18, 23, 16, 16, 16, 24, 19, 12, 12, 20, 3, 21, 13, 22, 3, 3, 35, 3, 42, 35, 3, 19, 12, 12
, 20, 3, 18, 3, 38, 3, 19, 12, 20, 18, 3, 3, 49, 3, 49, 3, 23, 16, 24, 19, 18, 12, 12, 20, 3, 18, 3, 3, 36
, 32, 3, 50, 52, 63, 64, 64, 64, 64, 65, 60, 62, 54, 52, 52, 52, 52, 53, 52, 52, 55, 57, 69, 70, 70, 70, 70
, 70, 71, 66, 68, 57, 57, 57, 69, 70, 71, 57, 57, 66, 68, 56, 52, 54, 52, 52, 52, 53, 53, 52, 52, 51, 3, 3
, 39, 3, 3, 3, 10, 3, 8, 34, 3, 3, 34, 3, 3, ]

route280 = [
3, 1, 3, 3, 18, 3, 19, 20, 18, 3, 34, 3, 17, 35, 3, 3, 11, 3, 17, 33, 17, 3, 3
, 18, 20, 32, 10, 50, 52, 52, 63, 64, 64, 64, 65, 52, 53, 63, 65, 52, 52, 52, 54, 52, 54, 52, 54, 52, 52
, 52, 51, 9, 80, 38, 3, 34, 3, 11, 18, 50, 52, 54, 54, 52, 51, 3, 3, 7, 2, 3, 3, 18, 23, 24, 3, 3, 3, 49
, 46, 3, 11, 18, 19, 12, 20, 3, 18, 3, 41, 42, 3, 3, 15, 19, 12, 12, 12, 20, 19, 20, 3, 18, 3, 41, 3, 41
, 41, 17, 3, 3, 39, 3, 3, 34, 3, 23, 16, 16, 24, 18, 3, 17, 3, 3, 3, 18, 3, 35, 3, 3, 34, 3, 3, 3, 3, 46
, 3, 34, 3, 19, 12, 12, 12, 20, 10, 15, 34, 3, 3, 3, 23, 16, 16, 16, 16, 24, 3, 3, 8, 3, 2, 3, 3, 3, 3, 18
, 50, 52, 54, 52, 53, 63, 64, 64, 64, 65, 52, 52, 54, 54, 52, 55, 57, 57, 57, 69, 70, 70, 70, 70, 71, 56
, 52, 52, 53, 53, 53, 54, 52, 52, 51, 3, 9, 80, 3, 18, 3, 3, 48, 47, 3, 3, 41, 11, 3, 18, 50, 53, 53, 52
, 63, 64, 64, 65, 60, 62, 53, 63, 64, 64, 64, 65, 52, 52, 54, 52, 55, 69, 70, 71, 56, 54, 54, 63, 64, 64
, 65, 63, 65, 55, 57, 57, 57, 57, 58, 57, 57, 57, 59, 57, 57, 58, 57, 57, 59, 58, 57, 66, 68, 56, 52, 52
, 54, 54, 52, 52, 54, 52, 53, 52, 52, 53, 53, 52, 63, 64, 64, 64, 64, 64, 64, 64, 64, 65, 52, 52, 51, 3
, 18, 50, 51, 3, 50, 51, 18, 50, 52, 53, 52, 54, 52, 51, 3, 11, 3, 3, 2, 33, 19, 20, 21, 22, 23, 24, 19
, 20, 21, 22, 23, 24, 19, 20, 21, 22, 23, 24, 33, 3, 3 ]

expert = [
   1, 3, 3, 17, 3, 38, 3, 17, 34, 18, 3, 3, 3, 46, 35, 18, 3, 28, 26, 29, 18, 3, 36, 34, 3, 3, 3, 38, 39
, 3, 9, 80, 18, 3, 35, 17, 3, 3, 15, 17, 49, 50, 52, 52, 63, 65, 54, 53, 52, 52, 53, 53, 52, 55, 57, 57
, 59, 66, 68, 56, 52, 54, 52, 60, 62, 54, 51, 18, 50, 63, 65, 51, 35, 3, 42, 17, 11, 18, 35, 72, 73, 79
, 79, 79, 74, 35, 9, 80, 3, 48, 3, 48, 3, 9, 80, 39, 3, 17, 37, 3, 28, 25, 25, 26, 29, 10, 3, 49, 3, 17
, 3, 34, 17, 83, 10, 10, 10, 48, 17, 10, 28, 29, 3, ]

standard = [
1, 3, 10, 10, 34, 3, 18, 35, 3, 19, 20, 3, 39, 3, 3, 48, 3, 17, 3, 34, 18, 17, 3, 3, 3, 18, 3, 34, 50
, 52, 52, 53, 63, 65, 52, 52, 53, 54, 51, 18, 3, 49, 48, 3, 19, 12, 20, 82, 3, 3, 15, 10, 32, 34, 3, 3
, 50, 54, 52, 52, 60, 62, 52, 52, 52, 52, 54, 54, 51, 3, 3, 35, 17, 3, 3, 21, 22, 3, 3, 33, 3, 23, 24
, 10, 10, 37, 3, 3, 49, 3, 38, 35, 3, 3, 10, 10, 28, 25, 25, 26, 29, 3, 34, 18, 83, 10, 48, 28, 29, 9, 80, 29, ]

beginner = [
    29, 1, 3, 3, 3, 18, 3, 34, 3, 3, 17, 38, 3, 3, 3, 50, 52, 52, 52, 53, 52, 60, 62, 52, 63, 52, 52, 53
, 52, 52, 53, 63, 65, 52, 52, 51, 9, 80, 17, 3, 18, 35, 3, 3, 35, 18, 35, 17, 3, 3, 39, 34, 3, 50, 60
, 62, 52, 54, 52, 54, 52, 63, 65, 52, 54, 51, 3, 84, 39, 3, 28, 29, 3, 35, 3, 34, 3, 3, 3, 10, 10
, 10, 17, 3, 40, 7, 82, 28, ]

routes = [ route101, route280, expert, standard, beginner ]

class Pair:
    def __init__(self, id, pair, unpacked, freq):
        self.id = id
        self.pair = pair
        self.unpacked = unpacked
        self.freq = freq

    def __str__(self):
        return f"{self.id} {self.pair} | {self.freq} {self.unpacked}"

    def __repr__(self):
        return self.__str__()
    
    def tokenize(self, route):
        new_route = []
        i = 0
        while i < len(route):
            if i < len(route) - 1 and (route[i], route[i+1]) == self.pair:
                new_route.append(self.id)
                i += 2
            else:
                new_route.append(route[i])
                i += 1

        return new_route

id = 100
pairs = []
while id < 255:
    pair_frequencies = {}
    for route in routes:
        for i in range(len(route) - 1):
            pair = (route[i], route[i + 1])
            if pair not in pair_frequencies:
                pair_frequencies[pair] = 0
            pair_frequencies[pair] += 1

    most_frequent_pair = max(pair_frequencies, key=pair_frequencies.get)
    frequency = pair_frequencies[most_frequent_pair]
    if frequency < 2:
        break

    mfp_l = most_frequent_pair[0]
    mfp_l_unpacked = [ mfp_l ] if mfp_l < 100 else pairs[mfp_l - 100].unpacked
    mfp_r = most_frequent_pair[1]
    mfp_r_unpacked = [ mfp_r ] if mfp_r < 100 else pairs[mfp_r - 100].unpacked
    mfp_unpacked = mfp_l_unpacked + mfp_r_unpacked

    pair = Pair(id, most_frequent_pair, mfp_unpacked, frequency)
    pairs.append(pair)
    id += 1
    routes = [ pair.tokenize(route) for route in routes ]

#for pair in pairs:
#    print(f"{pair.pair[0]} -> {pair.pair[1]}")
for pair in pairs:
    print("static u8 bpe_pair_" + str(pair.id) + "[] = { " + ','.join([str(x) for x in pair.unpacked]) + " ,0 };")
    #print(f"{pair.id} = {pair.unpacked}")
for route in routes:
    print(route)

relations = {}

def add_relation(a, b):
    a_thawed = []
    while a >= 100:
        a_thawed.append(a)
        a = pairs[a - 100].pair[1]
    a_thawed.append(a)
    
    b_thawed = []
    while b >= 100:
        b_thawed.append(b)
        b = pairs[b - 100].pair[0]
    b_thawed.append(b)

    for a in a_thawed:
        if a not in relations:
            relations[a] = set()
        for b in b_thawed:
            relations[a].add(b)

for pair in pairs:
    add_relation(pair.pair[0], pair.pair[1])
for route in routes:
    for i in range(len(route) - 1):
        add_relation(route[i], route[i + 1])

for i in range(0, 255):
    if i in relations:
        #print(f"{i} -> {relations[i]}")
        print("static u8 bpe_relations_" + str(i) + "[] = { " + ','.join([str(x) for x in sorted(relations[i])]) + " ,0 };")

for i in range(0, 255):
    if i in relations:
        print("bpe_relations_" + str(i) + ",")
    else:
        print("NULL,")

start = 3
while True:
    if start not in relations:
        print(f"No relations found for {start}")
        break
    next_steps = relations[start]
    print(f"From {start} you can go to: {next_steps}")
    start = random.choice(list(next_steps))
