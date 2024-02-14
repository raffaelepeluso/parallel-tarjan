# parallel-tarjan
A parallel version of Tarjan's algorithm to find SCCs (Strongly Connected Components) in a graph.

## Dependencies

* CMake 3.25
* Open MPI
* Python3
* Pipenv

## How to run

1. Create a build directory and launch cmake

   ```batch
   mkdir build
   cd build
   cmake ..
   ```

2. Generate executables with `make`
3. To generate the graphs for sequential and parallel execution run  `make generate_graphs`
4. To generate measures using the generated graphs run `make generate_measures`
5. To extract mean times and speedup curves from generated measures run `make extract_measures`

The generated graphs along with the scc found by the sequential and parallel executions of the programs can be found in the `data/` directory.
In the `measures/` directory are located the speedup curves computed, along with the execution times divided by problem size and optimization used.
