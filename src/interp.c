// gridr
//
// freeman.justin@gmail.com


#include "grid.h"

#define TOLERANCE	1e-6
#define ABS(x)		((x) < 0 ? -(x) : (x))
#define MAX(a,b)    ( ((a) > (b)) ? (a) : (b) )
#define MIN(a,b)    ( ((a) < (b)) ? (a) : (b) )

double evaluate_linear_quad_shape_function( double *xi, double *eta, double *pos_to_interp_at, int node ){

	return ( 0.25 * (1.0 + pos_to_interp_at[0] * xi[node]) * (1.0 + pos_to_interp_at[1] * eta[node]) );

}

double relative_difference(double a, double b)
{
	double c = ABS(a);
	double d = ABS(b);

	d = MAX(c, d);

	return d == 0.0 ? 0.0 : ABS(a - b) / d;
}


void init_xi_eta(e *E){

	// anti-clockwise element numbering
	// master element nodal positions
    //
    //  E->xi is the x coordinate
    //  E->eta is the y coordinate
    //
    //            ^
    // (-1,1)     |      (1,1)
    //    o----------------o
    //    |       |        |
    //    |     (0,0)------|- >
    //    |                |
    //    o----------------o
    // (-1,-1)           (1,-1)
    //

	E->xi[0] =	-1.0;
	E->xi[1] =	 1.0;
	E->xi[2] =	 1.0;
	E->xi[3] =	-1.0;

	E->eta[0] = -1.0;
	E->eta[1] = -1.0;
	E->eta[2] =  1.0;
	E->eta[3] =  1.0;

}

void calculate_interpolation_weights(element *el, double *xi, double *eta, double *pos){

	int i;
	double	pos_local[2];

	// transform from global interpolation position to local element coordinates
	pos_local[0] = (pos[0] - 0.5*(el->node_coord[1][0] + el->node_coord[0][0]))/( 0.5 * (el->node_coord[1][0]-el->node_coord[0][0]) );
	pos_local[1] = (pos[1] - 0.5*(el->node_coord[3][1] + el->node_coord[0][1]))/( 0.5 * (el->node_coord[3][1]-el->node_coord[0][1]) );

	for(i=0; i < 4; i++)
		el->interp_weights[i] = evaluate_linear_quad_shape_function( xi, eta, pos_local, i );

}


void interpolate_point(element *el, double *interp_value){

	int i;

	*interp_value = 0.0;
	for(i=0; i < 4; i++) {
		*interp_value += el->node_value[i] * el->interp_weights[i];
    }
}



void store_mesh(e *E, int nx, int ny){

	int i,j,el;

	el=0;

	for(i=0;i<nx;i++){
		E->msh.x[i] = E->ele[i].node_coord[0][0];
	}
	E->msh.x[nx] = E->ele[nx-1].node_coord[1][0];

	for(i=0;i<ny;i++){
		E->msh.y[i] = E->ele[i*nx].node_coord[0][1];
	}
	E->msh.y[ny] = E->ele[(ny-1)*nx].node_coord[3][1];
}

int	get_owner_element(e *E, double *pos){

	int		i;
	int		max_its;
	int		element;
	int		element_new;
	int		x_element;
	int		y_element;
	double	point;
	double	interval_size;
	int		check = 1;


	max_its = 20;

	// search over elements to determine which element contains this position
	// apply a binary search method on the elements

	// search in x
    i=0;
    element = (double)E->nx/2.0;
    element_new = 0;
	interval_size = E->nx/2.0;

	do{
		if( (relative_difference(pos[0],E->msh.x[element+1])<=TOLERANCE) || (relative_difference(pos[0],E->msh.x[element])<=TOLERANCE)
		   ||
		   (pos[0] <= E->msh.x[element+1]) && (pos[0] >= E->msh.x[element]) ){
			// found it
			element = element;
			break;
		}
		else if( E->msh.x[element] >= pos[0] ){ // means that the pos point is on the left of our current index
			element_new = ( (double)element - ceil(interval_size/2.0) );
			if(element_new < 0) element_new=0;
		}
		else{	// means that the pos point is on the right of our current index

			element_new = ceil( (double)element + (interval_size/2.0) );

			if(element_new >= E->nx){
				element_new=E->nx-1;
			}
		}
		interval_size = fabs(element_new - element);
		element = element_new;
		i++;
	}while(i<max_its);

	if(i>=max_its){
		printf("x get owner failed!\n");
		printf("pos = %f, %f\n", pos[0], pos[1]);
		printf("element = %d\n", element);
		printf("its = %d\n", i);

		printf(" (%.5f, %.5f) --------- (%.5f, %.5f)\n", E->ele[element].node_coord[3][0], E->ele[element].node_coord[3][1],
			   E->ele[element].node_coord[2][0], E->ele[element].node_coord[2][1]);
		printf("   |                              |\n");
		printf("   |                              |\n");
		printf("   |                              |\n");
		printf("   |                              |\n");
		printf(" (%.5f, %.5f) --------- (%.5f, %.5f)\n\n", E->ele[element].node_coord[0][0], E->ele[element].node_coord[0][1],
			   E->ele[element].node_coord[1][0], E->ele[element].node_coord[1][1]);

		exit(1);
	}

	// x_element stores the column that this point will be in
	// lets use this information to constrain our search in y

	x_element = element;
	element = ceil((double)E->ny/2.0);
	element_new = 0;//(double)E->ny/2.0;
	interval_size = E->ny/2.0;

	// now search in y
	i=0;
	do{
		if( (relative_difference(pos[1],E->msh.y[element+1])<=TOLERANCE) || (relative_difference(pos[1],E->msh.y[element])<=TOLERANCE)
		   ||
		   (pos[1] <= E->msh.y[element+1]) && (pos[1] >= E->msh.y[element]) ){
			// found it
			break;
		}

		if( (E->msh.y[element] >= pos[1]) ){
			element_new = ( (double)element - ceil(interval_size/2.0) ) ;
			if(element_new < 0) element_new=0;
		}
		else{
			element_new = ( (double)element + ceil(interval_size/2.0) );

			if(element_new >= E->ny) {
				element_new=E->ny-1;
			}
        }

		interval_size = fabs(element_new - element);
		element = element_new;


		i++;
	}while(i<max_its);

	if(i>=max_its){
		printf("y get owner failed!\n");
		printf("pos = %f, %f\n", pos[0], pos[1]);
		printf("its = %d\n", i);

		printf(" (%.5f, %.5f) --------- (%.5f, %.5f)\n", E->ele[element].node_coord[3][0], E->ele[element].node_coord[3][1],
			   E->ele[element].node_coord[2][0], E->ele[element].node_coord[2][1]);
		printf("   |                              |\n");
		printf("   |                              |\n");
		printf("   |                              |\n");
		printf("   |                              |\n");
		printf(" (%.5f, %.5f) --------- (%.5f, %.5f)\n\n", E->ele[element].node_coord[0][0], E->ele[element].node_coord[0][1],
			   E->ele[element].node_coord[1][0], E->ele[element].node_coord[1][1]);

		exit(1);
	}

	// the owner element for this point is element * x_element
	element = (element*(E->nx)) + x_element;

    /*
        printf("get owner finished\n");
		printf("pos = %f, %f\n", pos[0], pos[1]);
		printf("its = %d\n", i);

		printf(" (%.5f, %.5f) --------- (%.5f, %.5f)\n", E->ele[element].node_coord[3][0], E->ele[element].node_coord[3][1],
			   E->ele[element].node_coord[2][0], E->ele[element].node_coord[2][1]);
		printf("   |                              |\n");
		printf("   |                              |\n");
		printf("   |                              |\n");
		printf("   |                              |\n");
		printf(" (%.5f, %.5f) --------- (%.5f, %.5f)\n\n", E->ele[element].node_coord[0][0], E->ele[element].node_coord[0][1],
			   E->ele[element].node_coord[1][0], E->ele[element].node_coord[1][1]);
    */

	return element;
}

void print_elements(e *E){

	int i;

	for(i=0;i<E->nElements;i++){
		printf("Element %d:\n", i);
		printf("\tnode pos: 0 = (%f,%f) 1 = (%f,%f) 2 = (%f,%f) 3 = (%f,%f)\n", E->ele[i].node_coord[0][0], E->ele[i].node_coord[0][1],E->ele[i].node_coord[1][0], E->ele[i].node_coord[1][1],
               E->ele[i].node_coord[2][0], E->ele[i].node_coord[2][1],E->ele[i].node_coord[3][0], E->ele[i].node_coord[3][1]);
		printf("\tnode values: 0 = %f, 1 = %f, 2 = %f, 3 = %f\n", E->ele[i].node_value[0], E->ele[i].node_value[1], E->ele[i].node_value[2], E->ele[i].node_value[3]);

	}

}



void interp_bathy_on_grid(e *E){

    int i,j;
    int el;
    double  pos[2];
    //double  interp_value;


    // setup the interpolation source grid data structures
	E->nx = E->b.nlon-1; //E->nc.x-1;
	E->ny = E->b.nlat-1; //E->nc.y-1;
	E->nElements = E->nx*E->ny ;
	E->ele = malloc(E->nElements*sizeof(element));
	E->nodesPerEl = 4;

	E->msh.x = malloc((E->nx+1) * sizeof(double));
	E->msh.y = malloc((E->ny+1) * sizeof(double));

	init_xi_eta(E);

	el = 0;
	for(i=0;i<E->ny;i++){
		for(j=0;j<E->nx;j++){

			// set positions for this element
			// element node numbering is anti-clockwise
			E->ele[el].node_coord[0][0] = E->b.lon[j];
			E->ele[el].node_coord[0][1] = E->b.lat[i]; // 0

			E->ele[el].node_coord[1][0] = E->b.lon[j+1];
			E->ele[el].node_coord[1][1] = E->b.lat[i]; // 1

			E->ele[el].node_coord[2][0] = E->b.lon[j+1];
			E->ele[el].node_coord[2][1] = E->b.lat[i+1]; // 2

			E->ele[el].node_coord[3][0] = E->b.lon[j];
			E->ele[el].node_coord[3][1] = E->b.lat[i+1]; // 3

			// now set the nodal value for this element
			E->ele[el].node_value[0] = -E->b.field[i][j];
			E->ele[el].node_value[1] = -E->b.field[i][j+1];
			E->ele[el].node_value[2] = -E->b.field[i+1][j+1];
			E->ele[el].node_value[3] = -E->b.field[i+1][j];

			el++;
		}
	}

    // the store_mesh call is required for the function get_owner_element
	store_mesh(E, E->nx, E->ny);
    //print_elements(E);

    // for each rho grid point
    for(i=0;i<E->g.nY;i++){
        for(j=0;j<E->g.nX;j++){

            // get pos of grid point
            pos[0] = E->lon_rho[i][j];
            pos[1] = E->lat_rho[i][j];

            // find out which element this lies within
			el = get_owner_element(E, pos);

            calculate_interpolation_weights(&E->ele[el], E->xi, E->eta, pos);
			interpolate_point(&E->ele[el], &E->h[i][j]);

        }
    }
}
