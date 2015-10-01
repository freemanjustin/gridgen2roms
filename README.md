# gridgen2roms
ROMS grid generation via `gridgen`.

# Build
## Depends on

`gridgen2roms` requires the following packages, libraries and data:

* [netCDF-C](http://www.unidata.ucar.edu/software/netcdf/)
* [gridgen-c](https://github.com/sakov/gridgen-c)
* [gridutils](https://github.com/sakov/gridutils-c)
* [libXML2](http://www.xmlsoft.org/)
* Bathymetry data in netcdf format 

## Install
First istall the netcdf-c library, followed by gridutils-c then gridgen-c. For use with netcdf4 data files you will also need to install the [HDF5](https://www.hdfgroup.org/HDF5/) library. On some systems you might need to install libXML2.

To build `gridgen2rom`s you may need to modify the `Makefile` to include your install location of the netcdf library and headers (at runtime you may also need to modify your `$LD_LIBRARY_PATH` as well). To compile `gridgen2roms` type `make` at the top level directory. The binary, `gridgen2roms`, will be placed in the `bin` directory.

## Extras
Included in the source code package is a basic web browser hosted user interface that you may use to generate the control points for input to gridgen-c. To use this interface load the `web/app/index.html` page into a web browser. You will be presented with an OpenLayers map on which you can create and modify a polygon describing the outer boundary of the grid that will be created.

![slicer sample](https://raw.github.com/freemanjustin/gridgen2roms/master/docs/ol_example.png)

Start your polygon by clicking on the map. Subsequent mouse clicks on the map will add control points to the polygon.
Double click to finish editing the polygon. Once editing is complete, the list of control point locations will be written underneath the map element. Points may be moved by dragging and new points can be added in between existing points by clicking. These points represent part of the input required for `gridgen-c`.


# How to make a grid for use with roms via `gridgen2roms`
This example will take you through the steps requires to generate a grid that may be used with roms.

1. Create a list of control points. If you are using the web app included in the source distribution then save the output coordinate polygon to a text file. Lets call this file `polygon_points.txt`
2. Create a `gridgen-c` input file called `gridgen_input.txt` with the following structure.
```
input polygon_points.txt
output polygon.grid
nnodes 14
nx 300
ny 36
precision 1.0e-6
sigmas sigmas.0
rectangle rect.0
newton 1
```
3. Run `gridgen -v gridgen_input.txt`
4. Note the conformal modulus that `gridgen-c` reports as part of its standard output.
5. Edit `gridgen_input.txt` to make sure that `(nx - 1) / (ny - 1) = conformal modulus`. This will generate cells with aspect ratio 1.
6. Re-run `gridgen -v gridgen_input.txt` to update the output with the new `nx` and `ny` values.
7. 
