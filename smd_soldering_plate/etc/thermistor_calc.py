#!/bin/python

# NTC 3950 values
vals = [
    [100, 25],
    [10.6, 85],
    [6.7, 100],
    [3.85, 120],
    [1.77, 150],
    [0.99, 175],
    [0.58, 200],
    [0.36, 225],
    [0.23, 250],
    [0.153, 275],
    [0.122, 290],
    [0.105, 300],
]

up_resistor = 10
adc_res = 12
adc_range = 2**adc_res

print("adc_range=", adc_range)

for _, v in enumerate(vals):
    total_resistor = up_resistor + v[0]
    point_ohms = (total_resistor/adc_range)*1000
    perc = point_ohms / (100*total_resistor)
    print('{}, point_ohms={}, percent={}%'.format(v, point_ohms, perc*100))
