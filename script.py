import random

# with open("edge.bin", "wb") as f:
#     for x in range(10):
#         for y in range(10):
#             f.write(x.to_bytes(2, byteorder="little"))
#             f.write(y.to_bytes(2, byteorder="little"))

# with open("color.bin", "wb") as f:
#     temp = []
#     for x in range(10):
#         for y in range(10):
#             _x = random.randint(0, 10)
#             _y = random.randint(0, 10)
#             while (_x, _y) in temp:
#                 _x = random.randint(0, 10)
#                 _y = random.randint(0, 10)

#             f.write(_x.to_bytes(2))
#             f.write(_y.to_bytes(2))

with open("edge.bin", "wb") as f:
    for x in range(317):
        for y in range(317):
            f.write(x.to_bytes(2, byteorder="little"))
            f.write(y.to_bytes(2, byteorder="little"))

with open("color.bin", "wb") as f:
    temp = []
    for x in range(317):
        for y in range(317):
            _x = random.randint(0, 317)
            _y = random.randint(0, 317)
            while (_x, _y) in temp:
                _x = random.randint(0, 317)
                _y = random.randint(0, 317)

            f.write(_x.to_bytes(2, byteorder="little"))
            f.write(_y.to_bytes(2, byteorder="little"))
