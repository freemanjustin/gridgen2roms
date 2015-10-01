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

## Extras
Included in the source code package is a basic web browser implemented user interface that you may use to generate the control points for input to gridgen-c. To use this interface load the index.html page into a web browser. You will be presented with an OpenLayers map on which you can create and modify a polygon describing the outer boundary of the grid that will be created.

![slicer sample](https://raw.github.com/freemanjustin/gridgen2roms/master/docs/ol_example.png)

Start your polygon by clicking on the map. Subsequent mouse clicks on the map will add control points to the polygon. Points may be moved by dragging and new points can be added in between existing points by clicking.

Double click to finish editing the polygon. Once editing is complete, the list of control point locations will be written underneath the map element. These points represent part of the input required for gridgen-c.


# How to make a grid for use with roms
