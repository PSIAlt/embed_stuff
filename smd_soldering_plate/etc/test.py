import random

random.seed()

s = 0

for i in range(1000000):
    n = random.randrange(1, 7)
    #print(n)
    if n == 1:
        s += 60

print("sum=", s)
