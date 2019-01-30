# ASEP


It is avialable under the GNU GPLv3 license.  Questions may be sent to the author at stephendesalvo@gmail.com.

## How to Execute

There are two demos.  

The first one is a complete graph on 100 nodes (where actually nodes 0 and 99 are the source and sink and the 98 remaining nodes form a clique.)
```bash
./ASEP_demo_100_nodes_complete_graph_20_particles_10000_iterations
```

The second demo reads in the incidence matrix from a file in input/graph.txt
```bash
./ASEP_demo_graph_from_file_3_particles_10_iterations
```

## How to make more demos

This depends on the library desalvo_standard_library, which is a library I started many years ago and just recently updated for this repository.  You need to make sure to clone that repository as well in a sibling folder.  I.e., I have ASEP and desalvo_standard_library as two folders within the same directory.  Then you should be able to compile and run the file asep_main.cpp below in order to create more demos.

More demos can be made by editing asep_main.cpp and invoking
```bash
g++ -std=c++11 asep_main.cpp  -o my_demo.exe
./my_demo
```

## Documentation

All classes, namespaces, and functions are documented using Doxygen.
