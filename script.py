import random

with open("edge.bin", "wb") as f:
    for x in range(250):
        for y in range(250):
            f.write(x.to_bytes(1))
            f.write(y.to_bytes(1))

with open("color.bin", "wb") as f:
    temp = []
    for x in range(250):
        for y in range(250):
            _x = random.randint(0, 250)
            _y = random.randint(0, 250)
            while (_x, _y) in temp:
                _x = random.randint(0, 250)
                _y = random.randint(0, 250)

            f.write(_x.to_bytes(1))
            f.write(_y.to_bytes(1))
