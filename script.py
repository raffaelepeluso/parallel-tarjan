from pathlib import Path
import random
import struct

load = (24000, 32000, 41000, 48000)
proc = (1, 2, 4, 8)

Path('../data/seq').mkdir(parents=True, exist_ok=True)

for i in range(len(load)):
    num_vertices = load[i]
    integers = []
    with open("../data/seq/graph{}.bin".format(num_vertices), "wb") as f:
        print(f'Generate graph with {num_vertices} vertices')
        for j in range(num_vertices):
            integers.append(j)
            f.write(struct.pack("i", j))
            degree = random.randint(0, num_vertices - 1)
            integers.append(degree)
            f.write(struct.pack("i", degree))
            edges = random.sample(range(num_vertices), degree)
            integers.extend(edges)
            f.write(struct.pack("i" * len(edges), *edges))
    for k in range(len(proc)):
        num_processes = proc[k]
        Path('../data/par/graph{}/size{}'.format(num_vertices,
             num_processes)).mkdir(parents=True, exist_ok=True)
        vertices_per_process = num_vertices // num_processes
        bp = 0
        for l in range(num_processes):
            with open("../data/par/graph{}/size{}/process{}.bin".format(num_vertices, num_processes, l), "wb") as f:
                print(f'Generate graph with {num_vertices} vertices on {
                      num_processes} processes - process {l}')
                for _ in range(vertices_per_process):
                    id = integers[bp]
                    f.write(struct.pack("i", id))
                    bp += 1
                    degree = integers[bp]
                    f.write(struct.pack("i", degree))
                    bp += 1
                    for _ in range(degree):
                        f.write(struct.pack("i", integers[bp]))
                        bp += 1
