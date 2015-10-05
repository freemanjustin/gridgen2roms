# gridgen2roms
ROMS grid generation via `gridgen`.

# Build
## Depends on

`gridgen2roms` requires the following packages, libraries and data:

* [netCDF-C](http://www.unidata.ucar.edu/software/netcdf/)
* [gridgen-c](https://github.com/sakov/gridgen-c)
* [gridutils-c](https://github.com/sakov/gridutils-c)
* [libXML2](http://www.xmlsoft.org/)
* Bathymetry data in netcdf format 

## Install
First istall the netcdf-c library, followed by gridutils-c then gridgen-c. For use with netcdf4 data files you will also need to install the [HDF5](https://www.hdfgroup.org/HDF5/) library. On some systems you might need to install libXML2.

To build `gridgen2rom`s you may need to modify the `Makefile` to include your install location of the netcdf library and headers (at runtime you may also need to modify your `$LD_LIBRARY_PATH` as well). To compile `gridgen2roms` type `make` at the top level directory. The binary, `gridgen2roms`, will be placed in the `bin` directory.

## Extras
Included in the source code package is a simple, web browser hosted, user interface that you may use to create the control points for input to gridgen-c. To use this interface load the `web/app/index.html` page into a web browser. You will be presented with an OpenLayers map on which you can create and modify a polygon describing the outer boundary of the grid that will be created.

![grid UI sample](https://raw.github.com/freemanjustin/gridgen2roms/master/docs/ol_example.png)

Start your polygon by clicking on the map. Subsequent mouse clicks on the map will add control points to the polygon.

Double click to finish editing the polygon. Once editing is complete, the list of control point locations will be written underneath the map element. Points may be moved by dragging and new points can be added in between existing points by clicking. These points represent part of the input required for `gridgen-c`.


# How to make a roms grid with `gridgen2roms`
This example will take you through the steps to generate a roms grid.

## Polygon Definition

First, create a list of control points. Here is an example.

![sample region](https://raw.github.com/freemanjustin/gridgen2roms/master/docs/example_region.png)

If you are using the web app included in the source distribution then save the output coordinate polygon to a text file. In this example we will save the point list to a file named `polygon_points.txt`

```
149.95 -30.3 
155.9 -29.1 
153.45 -18.15 
142.2 -10.3 
139.05 -14.95 
148.2 -23.25 
geographic 148 -20
```

**Note** that I have omitted the last coordinate reported by the web app since it is a duplicate.

## beta values
We need to add some additional information to the polygon points to include and extra parameter required by `gridgen-c`. This parameter is referred to as `beta` and it defines angles of the polygons corners. The sum of `beta` must always equal 4. For the simple grid defined by the polygon above, we will prescribe the following beta values

![beta points](https://raw.github.com/freemanjustin/gridgen2roms/master/docs/beta.png)

Our updated `polygon_points.txt` file now looks like this

```
149.95 -30.3 1
155.9 -29.1 1
153.45 -18.15 
142.2 -10.3 1
139.05 -14.95 1
148.2 -23.25 
```

## `gridgen-c` Input

Create a `gridgen-c` input file called `gridgen_input.txt` with the following content.

```
input polygon_points.txt
output polygon.grid
nx 20
ny 20
newton 1
geographic 148 -20
```

More information about `gridgen-c` is available by running `gridgen -h` on the command line.

## Run gridgen

Run `gridgen -v gridgen_input.txt`. `gridgen-c` should produce the following output:

```
-> input = "polygon_points.txt"
reading:
  6 vertices after read
  counterclockwise ok
thinning:
  6 vertices after thinning
checking for self-intersections:
-> nnodes = "14"
nnodes = 14
-> newton = "1"
-> precision = "1.0e-6"
precision = 1e-06
-> nx = "20"
-> ny = "20"
going to generate 20x20 grid
-> output = "polygon.grid"
-> sigmas = "sigmas.0"
-> rectangle = "rect.0"
getting marked vertices:
  image region: 4 corners
triangulating:
  4 triangles
  9 edges
phase 1 split:
  6 vertices after phase 1 split
triangulating:
  4 triangles
  9 edges
phase 2 split:
  .
  6 vertices after phase 2 split
triangulating:
  4 triangles
  9 edges
calculating betas:
checking input:
getting quadrilaterals:
  3 diagonals
getting image region corner vertex indices:
calculating log|ro|:
solving for sigmas:
  .
setting new betas:
calculating image region:
  conformal modulus = 3.4635
saving image region:
mapping quadrilaterals (nppe = 3):
  .........
generating grid:
  .............o..ooo..............o..oooo.............o...o.o...............ooo.................oo...................o..................................................................................................................................................................................................................................................o...................o.................... (400 nodes)
```

## Cell Aspect Ratio

Note the conformal modulus that `gridgen-c` reports as part of its standard output. In the aboove example output, the conformal modulus is reported as
```
 conformal modulus = 3.4635
```
At this point the grid look like this:

![1st grid](https://raw.github.com/freemanjustin/gridgen2roms/master/docs/1st_grid.png)

Edit `gridgen_input.txt` to make sure that `(nx - 1) / (ny - 1) = conformal modulus`. This will generate cells with aspect ratio 1. Using our current `nx = 20` and `ny = 20` values gives `(20-1)/(20-1) = 1.0`. Using values of `nx = 32` and `ny = 10` will give us a ratio of `(32-1)/(10-1) = 3.44` which is close to the reported conformal modulus value reported by `gridgen-c`. 

## Update the `gridgen-c` input file
Update the `gridgen-c` input file to include the new `nx` and `ny` values
```
input polygon_points.txt
output polygon.grid
nx 32
ny 10
newton 1
geographic 148 -20
```

Re-run gridgen via `gridgen -v gridgen_input.txt` to update the output grid coordinates with the new `nx` and `ny` values.

```
-> input = "polygon_points.txt"
reading:
  6 vertices after read
  counterclockwise ok
thinning:
  6 vertices after thinning
checking for self-intersections:
-> nnodes = "14"
nnodes = 14
-> newton = "1"
-> precision = "1.0e-6"
precision = 1e-06
-> nx = "32"
-> ny = "10"
going to generate 32x10 grid
-> output = "polygon.grid"
-> sigmas = "sigmas.0"
-> rectangle = "rect.0"
getting marked vertices:
  image region: 4 corners
triangulating:
  4 triangles
  9 edges
phase 1 split:
  6 vertices after phase 1 split
triangulating:
  4 triangles
  9 edges
phase 2 split:
  .
  6 vertices after phase 2 split
triangulating:
  4 triangles
  9 edges
calculating betas:
checking input:
getting quadrilaterals:
  3 diagonals
getting image region corner vertex indices:
calculating log|ro|:
solving for sigmas:
  .
setting new betas:
calculating image region:
  conformal modulus = 3.4635
saving image region:
mapping quadrilaterals (nppe = 3):
  .........
generating grid:
  .....................oo...ooooo.......................o.o..ooo........................o..o............................o...............................o........................................................................................................................................o................................ (320 nodes)
```

The resulting grid from `gridgen-c` looks like this:

![updated grid](https://raw.github.com/freemanjustin/gridgen2roms/master/docs/2nd_grid.png)

## Make the roms grid
From the grid node coordinates generated by `gridgen-c` we can now generate a roms grid file. As part of the roms grid generation workflow, `gridgen2roms` will interpolate an input bathymetry onto the output grid. 

First, create an input xml file for `gridgen2roms` with the following contents

```
<grid>
    <bathymetry>
        <filename>etopo1.nc</filename>
        <lat_coord>lat</lat_coord>
        <lon_coord>lon</lon_coord>
        <field>z</field>
        <min_depth>5.0</min_depth>
    </bathymetry>
</grid>
```
In the above we have included a bathymetry data set filename as well as some additional information about how the data is arranged in the file such as the latitude and longitude coordinate variable names and the name of the bathymetry variable. The `min_depth` parameter is used by `gridgen2roms` to create land-sea masks.

## Run `gridgen2roms`
Create the roms grid via
```
./gridgen2roms polygon.grid example.xml roms_grid.nc
```
where `polygon.grid` is the output generated by `gridgen-c`, `exmaple.xml` is the input XML paramter file for `gridgen2roms` and `roms_grid.nc` is the name of the output roms grid file created by `gridgen2roms`.

# NOTES

### Polygon definition
I usually try start my polygon at the on the left edge and move in a clockwise direction.
### Spline
I am considering adding a preprocessor utility to generate additional control points for the ploygon using a spline interpolation. For now, it is easiest to manlually add additional points.
### Bathymetry
I cut out the region of interest from the source bathymetry data to reduce the memory footprint and runtime of `gridgen2roms`. Like [gridr](https://github.com/freemanjustin/gridr), `gridgen2roms` loads the whole dataset into memory.

You can create a cutout using the [NCO](http://nco.sourceforge.net/) tool `ncks` like this
```
ncks -d lat,-40.0,-5.0 -d lon,100.0,170.0 original_dataset.nc -o cutout.nv
```
### Ease of use
The grid generation process could be made easier with a little bit of effort. If there is enough interest then I can look into setting up a cloud hosted gridgeneration service.

