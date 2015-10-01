# gridgen2roms
ROMS grid generation via gridgen.

# Build
## Depends on

gridgen2roms requires the following packages, libraries and data:

* [netCDF-C](http://www.unidata.ucar.edu/software/netcdf/)
* [gridgen-c](https://github.com/sakov/gridgen-c)
* [gridutils](https://github.com/sakov/gridutils-c)
* [libXML2](http://www.xmlsoft.org/)
* Bathymetry data in netcdf format 

## Install
First istall the netcdf-c library, followed by gridutils-c then gridgen-c. For use with netcdf4 data files you will also need to install the [HDF5](https://www.hdfgroup.org/HDF5/) library. On some systems you might need to install libXML2.

To build gridgen2roms you may need to modify the Makefile to include your install location of the netcdf library and headers (at runtime you may also need to modify your $LD_LIBRARY_PATH as well). To compile gridgen2roms type make at the top level directory. The binary, gridgen2roms, will be placed in the bin directory.

# How to make a grid for use with roms
