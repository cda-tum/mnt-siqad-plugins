# QuickSim for SiQAD

QuickSim is an accurate *and* efficient physical simulation algorithm for determining the charge distribution of atomic *Silicon Dangling Bond (SiDB)* assemblies.
Since positively charged SiDBs are not yet used for logic, this algorithm is a 2-state simulation and neglects positively charged SiDBs, which is a reasonable assumption for SiDB assemblies without closely spaced SiDBs.

#### General information:
- Multithreading is still under development and hence, [SimAnneal](https://github.com/siqad/simanneal-sidb.git) must be run with ```instance count = 1 ```when comparing performance.
- QuickSim enables the tuning of simulation parameters: ``iterations steps`` and ``alpha.``For gate layouts, the default values are sufficient. If no solution is found, the number of iteration steps can be increased and alpha can be reduced.
- Only physically valid charge configurations are returned.

## Quick Start

> Clone the repository and its submodules:

```bash
git clone --recursive https://github.com/cda-tum/quicksim-siqad-plugin.git
```

## The CLI

> Inside the newly cloned `quicksim-siqad-plugin` folder, trigger the build process:

```bash
mkdir build 
```

> Run CMake (Debug modus reduces performance):

```bash
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=./release ..
```

```bash
cd build
make install
```

> Binary should be located in ```build``` .To test the binary, copy *sim_problem_0.xml* from ``` test ``` and *quicksim.physeng* and put it in ```build```.
> Start first simulation with example:

```bash
cd build
 ./quicksim sim_problem_0.xml sim_result_0.xml 
```

> Simulation results are stored in```sim_result_0.xml.```

### Using QuickSim in the SiQAD-GUI

> First step is to build [SiQAD](https://github.com/siqad/siqad). Afterwards, put the binary *quicksim* and *quicksim.physeng* from ```quicksim-siqad-plugin/build``` in ```build/siqad/plugins/quicksim```.

> SiQAD can be executed and ```QuickSim``` is available in the Engine list.

