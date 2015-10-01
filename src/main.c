// gridr
//
// freeman.justin@gmail.com


#include "grid.h"

int main(int argc,char **argv)
{
	e	*E;
    int     Lm,Mm,Lp,Mp, L, M;
    int     i,j;
    double  latdist;
    double  height, length;

    double  el, xl;

	FILE	*gridgen_input;
	char	str[64];


	// malloc the work struct
	E = malloc(sizeof(e));
	if(E==NULL) fail("Malloc failed\n");

	// parse command line arguments
	if(argc < 4){
		fail("need an input gridgen text file, input xml file and an output filename\n");
	}
	else{
		get_command_line_arg_as_string(&E->input_gridgen, argv[1]);
		get_command_line_arg_as_string(&E->input_xml, argv[2]);
		get_command_line_arg_as_string(&E->fname, argv[3]);
	}

    // read input XML
    get_params(E);

	// read the gridgen generated points file
	// first line is the grid dimensions
	gridgen_input = fopen(E->input_gridgen,"r");
	fgets( str, 240, gridgen_input );
	sscanf(str,"## %d x %d", &E->g.nX, &E->g.nY);
	//printf("nx = %d, ny = %d\n", E->g.nX, E->g.nY);

    // grid settings
    //E->g.nX = E->g.X/E->g.resol + 1;
    //E->g.nY = E->g.Y/E->g.resol + 1;

    //E->g.rotangle = E->g.rotangle/180.0*pi; // Convert Angle for grid rotation from degrees to radians
    //latdist  = spheriq_dist(E->g.lon,E->g.lat,E->g.lon,E->g.lat+1.0, 0); // Length (in meters) of 1 degree of latitude

    // malloc arrays
    malloc_arrays(E);

    Lm = E->g.nX-2;
    Mm = E->g.nY-2;
    Lp = Lm + 2;
    Mp = Mm + 2;
    L = Lp - 1;
    M = Mp -1;


    // RHO GRID
    // read in the rho grid points from the gridgen_input file
    for(i=0;i<=E->g.nY;i++){
        for(j=0;j<E->g.nX;j++){
            //E->x_rho[i][j] = E->x[j];
            //E->y_rho[i][j] = E->y[i];
			fgets( str, 240, gridgen_input );
			sscanf(str,"%lf %lf", &E->lon_rho[i][j], &E->lat_rho[i][j]);
			//E->lon_rho[i][j]+=360.0;
			//printf("%s, lon_rho = %f, lat_rho = %f\n",str, E->lon_rho[i][j], E->lat_rho[i][j]);
        }
    }

	fclose(gridgen_input);

    free(E->Rx_rho);
    free(E->Ry_rho);



    // U GRID
    // Create non-georeferenced grid in meters (origin = 0,0)

    for(i=0;i<E->g.nY;i++){
        for(j=0;j<E->g.nX-1;j++){
            E->lon_u[i][j] = 0.5*(E->lon_rho[i][j]+E->lon_rho[i][j+1]);
            E->lat_u[i][j] = 0.5*(E->lat_rho[i][j]+E->lat_rho[i][j+1]);
        }
    }




    free(E->Rx_u);
    free(E->Ry_u);
    free(E->x_u);
    free(E->y_u);


    // Calculate angle variable - angle at rho points
    for(i=0;i<E->g.nY-1;i++){
          for(j=0;j<E->g.nX;j++){

            E->angle[i][j] = bearing(E->lat_rho[i][j],E->lon_rho[i+1][j],E->lat_rho[i+1][j], E->lon_rho[i][j] );

        }
    }
	// fill in the last column
	for(j=0;j<E->g.nX;j++){
		E->angle[E->g.nY-1][j] = E->angle[E->g.nY-2][j];
	}

    // V GRID
    // Create non-georeferenced grid in meters (origin = 0,0)

    for(i=0;i<E->g.nY-1;i++){
        for(j=0;j<E->g.nX;j++){
            E->lon_v[i][j] = 0.5*(E->lon_rho[i][j]+E->lon_rho[i+1][j]);
            E->lat_v[i][j] = 0.5*(E->lat_rho[i][j]+E->lat_rho[i+1][j]);
        }
    }



    free(E->Rx_v);
    free(E->Ry_v);
    free(E->x_v);
    free(E->y_v);

    // PSI GRID
    // Create non-georeferenced grid in meters (origin = 0,0)

    for(i=0;i<E->g.nY-1;i++){
        for(j=0;j<E->g.nX-1;j++){
            E->lon_psi[i][j] = 0.5*(E->lon_rho[i][j]+E->lon_rho[i+1][j+1]);
            E->lat_psi[i][j] = 0.5*(E->lat_rho[i][j]+E->lat_rho[i+1][j+1]);
        }
    }


    free(E->x_rho);
    free(E->y_rho);
    free(E->x_psi);
    free(E->y_psi);
    free(E->Rx_psi);
    free(E->Ry_psi);


    // Grid spacing and other grid parameters

    el = E->lat_u[E->g.nY-1][0] - E->lat_u[0][0];
    xl = E->lon_v[0][E->g.nX-1] - E->lon_v[0][0];

    for(i=0;i<E->g.nY;i++){
        for(j=1;j<E->g.nX-1;j++){
            E->dx[i][j] = spheriq_dist(E->lon_u[i][j],E->lat_u[i][j],E->lon_u[i][j-1],E->lat_u[i][j-1],0);
        }
    }

    for(i=0;i<E->g.nY;i++){
        E->dx[i][0] = E->dx[i][1];
        E->dx[i][E->g.nX-1] = E->dx[i][E->g.nX-2];
    }

    for(i=1;i<E->g.nY-1;i++){
        for(j=0;j<E->g.nX;j++){
            E->dy[i][j] = spheriq_dist(E->lon_v[i][j],E->lat_v[i][j],E->lon_v[i-1][j],E->lat_v[i-1][j],0);
        }
    }

    for(i=0;i<E->g.nX;i++){
        E->dy[0][i] = E->dy[1][i];
        E->dy[E->g.nY-1][i] = E->dy[E->g.nY-2][i];
    }

    for(i=0;i<E->g.nY;i++){
        for(j=0;j<E->g.nX;j++){
            E->pm[i][j] = 1.0/E->dx[i][j];
            //printf("pm[%d][%d] = %g\n", i, j, pm[i][j]);
            E->pn[i][j] = 1.0/E->dy[i][j];
        }
    }

    for(i=0;i<E->g.nY;i++){
        for(j=0;j<E->g.nX;j++){
            E->dndx[i][j] = E->dmde[i][j] = 0.0;
        }
    }

    for(i=1;i<E->g.nY-1;i++){
        for(j=1;j<E->g.nX-1;j++){
            E->dndx[i][j] = 0.5*(1.0/E->pn[i][j+1] - 1.0/E->pn[i][j-1]);
            E->dmde[i][j] = 0.5*(1.0/E->pm[i+1][j] - 1.0/E->pm[i-1][j]);
        }
    }

    // Coriolis
    // f = 2 .* 7.29E-5 .* sin(lat_rho .* (pi/180)); %Estimation of Coriolis over the grid domain. OMEGA=7.29E-5
    // More info: http://en.wikipedia.org/wiki/Coriolis_effect#Formula
    for(i=0;i<E->g.nY;i++){
        for(j=0;j<E->g.nX;j++){
           E->f[i][j] = 2.0 * 7.29E-5 * sin(E->lat_rho[i][j] * (M_PI/180.0));
        }
    }

    // interpolate bathymetry
    interp_bathy_on_grid(E);
    // free bathymetry memory
    free(E->b.field);
    free(E->b.lon);
    free(E->b.lat);

    // generate land sea mask
    // 0 == land
    // 1 == water

    // Land/Sea mask on RHO-points
    // h[i][j] is now positive
    for(i=0;i<E->g.nY;i++){
        for(j=0;j<E->g.nX;j++){
            // original
            //E->mask_rho[i][j] = E->h[i][j] < E->b.min_depth ? 0 : 1;
            // psandery mod - make wet cells in the bathymetry wet in the mask
            E->mask_rho[i][j] = E->h[i][j] < 0.0 ? 0 : 1;
        }
    }

    // Land/Sea mask on U-points.
    for(i=0;i<E->g.nY;i++){
        for(j=0;j<E->g.nX-1;j++){
            E->mask_u[i][j] = E->mask_rho[i][j] * E->mask_rho[i][j+1];
        }
    }

    //  Land/Sea mask on V-points.
    for(i=0;i<E->g.nY-1;i++){
        for(j=0;j<E->g.nX;j++){
            E->mask_v[i][j] = E->mask_rho[i][j] * E->mask_rho[i+1][j];
        }
    }

    // Land/Sea mask on PSI-points.
    for(i=0;i<E->g.nY-1;i++){
        for(j=0;j<E->g.nX-1;j++){
            E->mask_psi[i][j] = E->mask_rho[i][j] * E->mask_rho[i+1][j];
        }
    }

    // apply min depth to bathymetry
    for(i=0;i<E->g.nY;i++){
        for(j=0;j<E->g.nX;j++){
            E->h[i][j] = E->h[i][j] < E->b.min_depth ? E->b.min_depth : E->h[i][j];
            // make bathymetry positive for ROMS
            //E->h[i][j] = fabs(E->h[i][j]);
        }
    }

    // smooth bathymetry?

    // save netcdf grid file
    write_netcdf(E);

	return 0;
}
