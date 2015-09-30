// gridr
//
// freeman.justin@gmail.com


// standard headers
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>

// netCDF header
#include <netcdf.h>

// libxml2 headers
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "jutil.h"


// macros
#define	TRUE 1
#define FALSE 0


#define max(x,y) (((x) > (y)) ? (x) : (y))
#define min(x,y) (((x) < (y)) ? (x) : (y))


#define fail(...) grid_fail(__LINE__,__func__,__FILE__,__VA_ARGS__)

#define pi  (M_PI)
#define earthradius (6356750.52)
#define deg2rad (M_PI/180.0)
#define rad2deg (180.0/M_PI)


// interpolation structs
typedef struct{
	double	node_coord[4][2];
	double	node_value[4];
	double	interp_weights[4];
}element;

typedef struct{
	double	*x;
	double	*y;
}mesh;


// grid parameters
typedef struct{
    double  lat;        // Latitude  (degrees) of the bottom-left corner of the grid.
    double  lon;        // Longitude (degrees) of the bottom-left corner of the grid.

    double  X;          // Width of domain (meters)
    double  Y;          // Length of domain (meters)
    double  rotangle;   // Angle (degrees) to rotate the grid conterclock-wise
    double  resol;      // Cell width and height (i.e. Resolution)in meters. Grid cells are forced to be (almost) square.
    int     N;          // Number of vertical levels

    int     nX;         // number of X
    int     nY;         // number of Y

    char    *Grid_filename;
}grid;

// bethymetry netcdf params
typedef struct{
    char    *fname;
    char    *lat_name;
    char    *lon_name;
    char    *field_name;

    double  **field;
    double  *lat;
    double  *lon;

    double  min_depth;

    // index variables
    size_t      nlat;
    size_t      nlon;


}bathymetry;

typedef struct{

	char	*input_gridgen;
	char    *input_xml;
    char    *fname;
    grid    g;
    bathymetry b;

    // output grid parameters
    int     one;

    char    spherical[1];

    double  *x,*y;
    double  **x_rho, **y_rho;
    int     Lm,Mm,Lp,Mp, L, M;
    int     i,j;
    double  latdist;

    double  el, xl;

    double  **dx, **dy;
    double  **pm, **pn;
    double  **dndx;
    double  **dmde;
    double  **f;
    double  **h;    // bathymetry on rho grid

    double  **Rx_rho;
    double  **Ry_rho;

    double  **lat_rho, **lon_rho;
    double  **x_u,**y_u;
    double  **x_v,**y_v;
    double  **x_psi,**y_psi;
    double  **Rx_u,**Ry_u;
    double  **Rx_v,**Ry_v;
    double  **Rx_psi,**Ry_psi;
    double  **lat_u, **lon_u;
    double  **lat_v, **lon_v;
    double  **lat_psi, **lon_psi;
    double  **mask_rho;
    double  **mask_u;
    double  **mask_v;
    double  **mask_psi;
    double  **angle;

    // netcdf params
    int ncid;
    int retval;
    int dimIdsRho[NC_MAX_VAR_DIMS];
    int dimIdsU[NC_MAX_VAR_DIMS];
    int dimIdsV[NC_MAX_VAR_DIMS];
    int dimIdsPsi[NC_MAX_VAR_DIMS];
    int dimIdsOne[NC_MAX_VAR_DIMS];

    // dimensions
    int xi_rho_dimid;
    int xi_u_dimid;
    int xi_v_dimid;
    int xi_psi_dimid;
    int eta_rho_dimid;
    int eta_u_dimid;
    int eta_v_dimid;
    int eta_psi_dimid;
    int one_dimid;

    // variable ids
    int vid_angle;
    int vid_dmde;
    int vid_dndx;
    int vid_el;
    int vid_f;
    int vid_h;
    int vid_lat_rho;
    int vid_lat_psi;
    int vid_lat_u;
    int vid_lat_v;

    int vid_lon_rho;
    int vid_lon_psi;
    int vid_lon_u;
    int vid_lon_v;

    int vid_mask_rho;
    int vid_mask_psi;
    int vid_mask_u;
    int vid_mask_v;

    int vid_pm;
    int vid_pn;
    int vid_spherical;
    int vid_xl;
    int vid_X;
    int vid_Y;
    int vid_dx;
    int vid_dy;


    //interp vars
    int		nElements;
	int		nodesPerEl;
	int		nx;
	int		ny;

	// anti-clockwise element numbering
	double	xi[4];
	double	eta[4];

	double	pos[2];

	element	*ele;
	mesh	msh;

}e;




// prototypes
// fail.c
void grid_fail( const int, const char*, const char*, const char*, ... );

void malloc_arrays( e* );
double spheriq_dist( double, double, double, double, int );
double distance(double, double, double, double);
double bearing(double, double, double, double );


// netcdf functions

void write_netcdf( e* );
void create_netcdf( e*, char*, int* );
void defdims_netcdf( e* );
void defvars( e* );
void defvar_netcdf(e *E, int, char*, nc_type, int, int*, int*);
void add_txt_attribute_netcdf(e *E, int, int, char*, char* );

void add_global_metadata( e*, int );
void write_data( e* );

void get_params( e* );
void _parseInputFile_params ( e*, xmlDocPtr, xmlNodePtr );
void _parseInputFile_bathymetry( e*, xmlDocPtr, xmlNodePtr );

// interp functions
void init_xi_eta(e *E);
void calculate_interpolation_weights(element*, double*, double*, double*);
void interpolate_point(element*, double*);
void store_mesh(e*, int, int );
int	get_owner_element(e*, double*);
double evaluate_linear_quad_shape_function( double*, double*, double *, int );
double relative_difference(double, double);
void interp_bathy_on_grid(e*);
