## Running CAPMD with python scripts

To build and instal CAPMD: 

```
git clone https://github.com/nrstillman/CAPMD.git
```

To run capmd with python:

```
mkdir build
cd build
cmake ..
make
```

then add following to bashrc:

```
export PYTHONPATH="user/path/to/capmd/build":"${PYTHONPATH}"
```
