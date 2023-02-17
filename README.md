# QuickSim for SiQAD

QuickSim is an accurate *and* efficient physical simulation algorithm for determining the charge distribution of atomic *Silicon Dangling Bond (SiDB)* assemblies.
Since positively charged SiDBs are not yet used for logic, this algorithm is a 2-state simulation and neglects positively charged SiDBs, which is a reasonable assumption for SiDB assemblies without adjacent SiDBs.

## Compilation

### Compiling C++ Binaries with CMake

First, boost has to be installed *(e.g. mac: brew install boost)*.

```bash
mkdir build && cd build
```

Run CMake:

```bash
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=./release ..
make install
```

Binary should be located in ```build```. 

To test the binary, copy```sim_problem_0.xml```from```test```and```quicksim.physeng```and put it in```build```.
Go to```build```and execute:

```bash
 ./quicksim sim_problem_0.xml sim_result_0.xml 
```

Simulation results are stored in```sim_result_0.xml.```

### Using QuickSim in the SiQAD-GUI

Build the SiQAD-Project (more information in the corresponding repository). Afterwards, put the binary *quicksim* and ```quicksim.physeng``` in ```build/siqad/plugins/quicksim```.

