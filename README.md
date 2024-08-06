**Work in progress**

# garmi-head

[![Actions Status][actions-badge]][actions-link]
[![Documentation Status][rtd-badge]][rtd-link]

[![PyPI version][pypi-version]][pypi-link]
[![Conda-Forge][conda-badge]][conda-link]
[![PyPI platforms][pypi-platforms]][pypi-link]

[![GitHub Discussion][github-discussions-badge]][github-discussions-link]

## Install

This module is unfortunately not portable and as such needs to be build from source locally (see this [post](https://github.com/tum-robotics/garmi-head/pull/4#issuecomment-2272021433) for details).

## Build

You need to build and setup the IgH EtherCat Master. You can do this by using the script included in this repository:

```
./bin/install_ehtercat <master0_device>
```
where \<master0_device\> is the MAC address or device name of your ethernet master, e.g. `94:c6:91:ae:d1:f5` or `eno1`.
The script should work for modern Ubuntu versions (22.04+) and sets up permissions, configuration files etc. You can
also take a look at the code and run through it step by step.
Next install the head drivers, i.e. the file `tum_head-2.1.0-Linux.deb`.
You can now install the Python module from this repository by executing:
```
pip install .
```

<!-- SPHINX-START -->

<!-- prettier-ignore-start -->
[actions-badge]:            https://github.com/tum-robotics/garmi-head/workflows/CI/badge.svg
[actions-link]:             https://github.com/tum-robotics/garmi-head/actions
[conda-badge]:              https://img.shields.io/conda/vn/conda-forge/garmi-head
[conda-link]:               https://github.com/conda-forge/garmi-head-feedstock
[github-discussions-badge]: https://img.shields.io/static/v1?label=Discussions&message=Ask&color=blue&logo=github
[github-discussions-link]:  https://github.com/tum-robotics/garmi-head/discussions
[pypi-link]:                https://pypi.org/project/garmi-head/
[pypi-platforms]:           https://img.shields.io/pypi/pyversions/garmi-head
[pypi-version]:             https://img.shields.io/pypi/v/garmi-head
[rtd-badge]:                https://readthedocs.org/projects/garmi-head/badge/?version=latest
[rtd-link]:                 https://garmi-head.readthedocs.io/en/latest/?badge=latest

<!-- prettier-ignore-end -->
