import FuzzyCython
import time

#print(FuzzyCython.JaroSimilarity(x, y))
print(FuzzyCython.Version())

test = [('asdasdas', 'gasdfa'), ('2uiqeuweioh', 'qioweyuiohscn'), ('funnels', 'tunnesl'),
        ('klh890uaplsjd', 'aospduapksndipkl')]
for i in range(1000):
    x = str(time.time())
    y = str(time.time())
    test.append((x, y))

for i in test:
    x = i[0]
    y = i[1]
    res1 = FuzzyCython.JaccardIndex(x,y)
    n = set(x)
    o = set(y)
    res2 = len(n.intersection(o)) / len(n.union(o))
    if (res1 != res2):
        print('{}\t{}\n'.format(x, y))
        print('{}\t{}\n'.format(res1, res2))
