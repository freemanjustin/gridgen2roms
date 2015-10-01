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

1. Create a list of control points like in the following example.

![sample region](https://raw.github.com/freemanjustin/gridgen2roms/master/docs/example_region.png)

If you are using the web app included in the source distribution then save the output coordinate polygon to a text file. Lets call this file `polygon_points.txt`
```
149.95 -30.3 
155.9 -29.1 
153.45 -18.15 
142.2 -10.3 
139.05 -14.95 
148.2 -23.25 
```

Note that I have omitted the last coordinate reported by the web app since it is a duplicate.

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

2. Create a `gridgen-c` input file called `gridgen_input.txt` with the following structure.
```
input polygon_points.txt
output polygon.grid
nnodes 14
nx 30
ny 10
precision 1.0e-6
sigmas sigmas.0
rectangle rect.0
newton 1
```
3. Run `gridgen -v gridgen_input.txt`. `gridgen-c` should produce the following output:

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
-> nx = "30"
-> ny = "10"
going to generate 30x10 grid
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
  ............
  saving sigmas to "sigmas.0":
setting new betas:
calculating image region:
  conformal modulus = 3.4635
saving image region:
mapping quadrilaterals (nppe = 3):
  .........
generating grid:
  ....................o...ooooo.....................o....ooo.........................o.........................................................o...............................................................................................................................o.............................. (300 nodes)
```

4. Note the conformal modulus that `gridgen-c` reports as part of its standard output. In the aboove example output, the conformal modulus is reported as

```
 conformal modulus = 3.4635
```

At this point the grid locations look like this

![1st grid](https://raw.github.com/freemanjustin/gridgen2roms/master/docs/1st_grid.png)

5. Edit `gridgen_input.txt` to make sure that `(nx - 1) / (ny - 1) = conformal modulus`. This will generate cells with aspect ratio 1. Using our current `nx = 30` and `ny = 10` values gives `(30-1)/(10-1) = 3.22`. An improved `nx` and `ny` of `nx = 32` and `ny = 10` will give us a ratio of `(32-1)/(10-1) = 3.44` which is close to the reported conformal modulus value reported by `gridgen-c`. 

6. Update the `gridgen-c` input file to include the new `nx` and `ny` values

```
input polygon_points.txt
output polygon.grid
nnodes 14
nx 32
ny 10
precision 1.0e-6
sigmas sigmas.0
rectangle rect.0
newton 1
```


7. Re-run `gridgen -v gridgen_input.txt` to update the output grid coordinates with the new `nx` and `ny` values.
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

The resulting grid from `gridgen-c` is

![updated grid](https://raw.github.com/freemanjustin/gridgen2roms/master/docs/2nd_grid.png)
