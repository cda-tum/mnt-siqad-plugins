# QuickSim Plugin for SiQAD

[![Ubuntu CI](https://img.shields.io/github/actions/workflow/status/cda-tum/mnt-siqad-plugin/ubuntu.yml?label=Ubuntu&logo=ubuntu&style=flat-square)](https://github.com/cda-tum/mnt-siqad-plugin/actions/workflows/ubuntu.yml)
[![macOS CI](https://img.shields.io/github/actions/workflow/status/cda-tum/mnt-siqad-plugin/macos.yml?label=macOS&logo=apple&style=flat-square)](https://github.com/cda-tum/mnt-siqad-plugin/actions/workflows/macos.yml)
[![Windows CI](https://img.shields.io/github/actions/workflow/status/cda-tum/mnt-siqad-plugin/windows.yml?label=Windows&logo=windows&style=flat-square)](https://github.com/cda-tum/mnt-siqad-plugin/actions/workflows/windows.yml)
[![arXiv](https://img.shields.io/static/v1?label=arXiv&message=2303.03422&color=informational&style=flat-square)](http://arxiv.org/abs/2303.03422)

<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="docs/_static/mnt_light.svg" width="60%">
    <img src="docs/_static/mnt_dark.svg" width="60%">
  </picture>
</p>

*QuickSim* is an accurate *and* efficient physical simulation algorithm for determining the charge distribution of
atomic *Silicon Dangling Bond (SiDB)* assemblies. Since positively charged SiDBs are not yet used for logic, this
algorithm is a 2-state simulation and neglects positively charged SiDBs, which is a reasonable assumption for SiDB
assemblies without closely spaced SiDBs.

QuickSim is implemented on top of the [*fiction*](https://github.com/marcelwa/fiction/) framework. This plugin makes the
algorithm available inside the [SiQAD](https://github.com/siqad/siqad) GUI.

#### General information:

- QuickSim enables the tuning of simulation parameters: `iteration steps` and `alpha`. For gate layouts, there should be
  no need to adjust the default values. If no solution is found, however, `iteration steps` can be increased
  or `alpha` reduced to enable high-effort simulation.
- QuickSim returns physically valid charge configurations only.

## Quick Start

All you need is a `git` client, `cmake`, the [`Boost`](https://www.boost.org/) libraries, and a modern C++ compiler.

### Building the Plugin

> Clone the repository and its submodules:

```bash
git clone --recursive https://github.com/cda-tum/mnt-siqad-plugin.git
```

> Inside the newly cloned `quicksim-siqad-plugin` folder, trigger the build process:

```bash
cmake . -B build
cd build
cmake --build . -j4
```

### Using QuickSim in the SiQAD GUI

Make sure you have [SiQAD](https://github.com/siqad/siqad) installed and the QuickSim plugin compiled.

Navigate to `<PATH-TO-SIQAD>/plugins/` and create a new folder called `quicksim`.

Move the `quicksim` binary and the `quicksim.physeng` file from `mnt-siqad-plugin/build/` to the newly
created`siqad/plugins/quicksim/` and restart SiQAD.

QuickSim is now available in the engine list.

## References

If you use __QuickSim__ in your work, we would appreciate it if you cite the following publication:

```bibtex
@INPROCEEDINGS{drewniok2023quicksimIEEE,
  title={{\emph{QuickSim}: Efficient \emph{and} Accurate Physical Simulation of Silicon Dangling Bond Logic}},
  author={Drewniok, Jan and Walter, Marcel and Ng, Samuel Sze Hang and Walus, Konrad and Wille, Robert},
  booktitle={IEEE International Conference on Nanotechnology (IEEE-NANO)},
  year={2023},
}
```
