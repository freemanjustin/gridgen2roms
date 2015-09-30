// gridr
//
// freeman.justin@gmail.com



#include "grid.h"



void write_netcdf(e *E){

	//float	fillValue = -1e34;
	float fillValue = NC_FILL_FLOAT;
	
    
	create_netcdf(E, E->fname, &E->ncid);
    
    defdims_netcdf(E);
	
	// setup variables
	defvars(E);
    
    	
	/*
	// add global metadata
	add_global_metadata(E, E->ncid);
	*/
    
    
    // write the data to the netcdf file
    write_data(E);
    
    nc_close(E->ncid);
}


void create_netcdf(e *E, char *fname, int *ncid){
	
	//int	old_fill_mode;
    
	if ( (E->retval = nc_create(fname, NC_CLOBBER, ncid) ) )
		fail("couldn't create netcdf out file %s\n",fname);
    
	// set the fill mode to be NO_FILL 
	//nc_set_fill(*ncid, NC_NOFILL, &old_fill_mode); 
	
}

void defdims_netcdf(e *E){
	
	E->one = 1;
	
	// define the dimensions
	if ((E->retval = nc_def_dim(E->ncid, "xi_rho", E->g.nX, &E->xi_rho_dimid)))
		fail("nc_def_dim failed!\n");
	
	if ((E->retval = nc_def_dim(E->ncid, "xi_u", E->g.nX-1, &E->xi_u_dimid)))
		fail("nc_def_dim failed!\n");
    
    if ((E->retval = nc_def_dim(E->ncid, "xi_v", E->g.nX, &E->xi_v_dimid)))
		fail("nc_def_dim failed!\n");
	
    if ((E->retval = nc_def_dim(E->ncid, "xi_psi", E->g.nX-1, &E->xi_psi_dimid)))
		fail("nc_def_dim failed!\n");
	
    if ((E->retval = nc_def_dim(E->ncid, "eta_rho", E->g.nY, &E->eta_rho_dimid)))
		fail("nc_def_dim failed!\n");
	
	if ((E->retval = nc_def_dim(E->ncid, "eta_u", E->g.nY, &E->eta_u_dimid)))
		fail("nc_def_dim failed!\n");
    
    if ((E->retval = nc_def_dim(E->ncid, "eta_v", E->g.nY-1, &E->eta_v_dimid)))
		fail("nc_def_dim failed!\n");
    
    if ((E->retval = nc_def_dim(E->ncid, "eta_psi", E->g.nY-1, &E->eta_psi_dimid)))
		fail("nc_def_dim failed!\n");

	
    if ((E->retval = nc_def_dim(E->ncid, "one", E->one, &E->one_dimid)))
		fail("nc_def_dim failed!\n");
    
	// end define mode for this file
	if ((E->retval = nc_enddef(E->ncid)))
      	fail("nc_enddef failed\n");
    
}


void defvars(e *E){
	
    // setup dimids
    
    E->dimIdsOne[0] = E->one_dimid;
    
    E->dimIdsRho[0] = E->eta_rho_dimid;
    E->dimIdsRho[1] = E->xi_rho_dimid;
    
    E->dimIdsPsi[0] = E->eta_psi_dimid;
    E->dimIdsPsi[1] = E->xi_psi_dimid;
    
    E->dimIdsU[0] = E->eta_u_dimid;
    E->dimIdsU[1] = E->xi_u_dimid;
    
    E->dimIdsV[0] = E->eta_v_dimid;
    E->dimIdsV[1] = E->xi_v_dimid;
    
    
	//float	fillValue = -1e34;
    defvar_netcdf(E, E->ncid, "angle", NC_DOUBLE, 2, &E->dimIdsRho[0], &E->vid_angle);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_dmde, "long_name", "angle between xi axis and east");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_dmde, "units", "radian");
	
	defvar_netcdf(E, E->ncid, "dmde", NC_DOUBLE, 2, &E->dimIdsRho[0], &E->vid_dmde);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_dmde, "long_name", "eta derivative of inverse metric factor pm");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_dmde, "units", "m");
    

	defvar_netcdf(E, E->ncid, "dndx", NC_DOUBLE, 2, &E->dimIdsRho[0], &E->vid_dndx);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_dndx, "long_name", "xi derivative of inverse metric factor pn");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_dndx, "units", "m");
    
    
	defvar_netcdf(E, E->ncid, "el", NC_DOUBLE, 1, &E->dimIdsOne[0], &E->vid_el);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_el, "long_name", "domain length in the ETA-direction");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_el, "units", "degrees");
    

	defvar_netcdf(E, E->ncid, "f", NC_DOUBLE, 2, &E->dimIdsRho[0], &E->vid_f);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_f, "long_name", "Coriolis parameter at RHO-points");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_f, "units", "second-1");
    
	defvar_netcdf(E, E->ncid, "h", NC_DOUBLE, 2, &E->dimIdsRho[0], &E->vid_h);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_h, "long_name", "bathmetry at RHO-points");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_h, "units", "m");
    
	defvar_netcdf(E, E->ncid, "lat_rho", NC_DOUBLE, 2, E->dimIdsRho, &E->vid_lat_rho);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_lat_rho, "long_name", "latitude at RHO-points");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_lat_rho, "units", "degree_north");
    add_txt_attribute_netcdf(E, E->ncid, E->vid_lat_rho, "_CoordinateAxisType", "Lat");
    
    
    
	defvar_netcdf(E, E->ncid, "lat_psi", NC_DOUBLE, 2, &E->dimIdsPsi[0], &E->vid_lat_psi);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_lat_psi, "long_name", "latitude at PSI-points");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_lat_psi, "units", "degree_north");
    add_txt_attribute_netcdf(E, E->ncid, E->vid_lat_psi, "_CoordinateAxisType", "Lat");
    
    
	defvar_netcdf(E, E->ncid, "lat_u", NC_DOUBLE, 2, &E->dimIdsU[0], &E->vid_lat_u);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_lat_u, "long_name", "latitude at U-points");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_lat_u, "units", "degree_north");
    add_txt_attribute_netcdf(E, E->ncid, E->vid_lat_u, "_CoordinateAxisType", "Lat");
    
    
	defvar_netcdf(E, E->ncid, "lat_v", NC_DOUBLE, 2, &E->dimIdsV[0], &E->vid_lat_v);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_lat_v, "long_name", "latitude at V-points");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_lat_v, "units", "degree_north");
    add_txt_attribute_netcdf(E, E->ncid, E->vid_lat_v, "_CoordinateAxisType", "Lat");
    
	defvar_netcdf(E, E->ncid, "lon_rho", NC_DOUBLE, 2, &E->dimIdsRho[0], &E->vid_lon_rho);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_lon_rho, "long_name", "longitude at RHO-points");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_lon_rho, "units", "degree_east");
    add_txt_attribute_netcdf(E, E->ncid, E->vid_lon_rho, "_CoordinateAxisType", "Lon");
    
    
	defvar_netcdf(E, E->ncid, "lon_psi", NC_DOUBLE, 2, &E->dimIdsPsi[0], &E->vid_lon_psi);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_lon_psi, "long_name", "longitude at PSI-points");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_lon_psi, "units", "degree_east");
    add_txt_attribute_netcdf(E, E->ncid, E->vid_lon_psi, "_CoordinateAxisType", "Lon");
    
    
    
	defvar_netcdf(E, E->ncid, "lon_u", NC_DOUBLE, 2, &E->dimIdsU[0], &E->vid_lon_u);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_lon_u, "long_name", "longitude at U-points");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_lon_u, "units", "degree_east");
    add_txt_attribute_netcdf(E, E->ncid, E->vid_lon_u, "_CoordinateAxisType", "Lon");
    
	defvar_netcdf(E, E->ncid, "lon_v", NC_DOUBLE, 2, &E->dimIdsV[0], &E->vid_lon_v);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_lon_v, "long_name", "longitude at V-points");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_lon_v, "units", "degree_east");
    add_txt_attribute_netcdf(E, E->ncid, E->vid_lon_v, "_CoordinateAxisType", "Lon");
    
    
	defvar_netcdf(E, E->ncid, "mask_rho", NC_DOUBLE, 2, &E->dimIdsRho[0], &E->vid_mask_rho);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_mask_rho, "long_name", "mask at RHO-points");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_mask_rho, "option_0", "land");
    add_txt_attribute_netcdf(E, E->ncid, E->vid_mask_rho, "option_1", "water");
    
    
	defvar_netcdf(E, E->ncid, "mask_psi", NC_DOUBLE, 2, &E->dimIdsPsi[0], &E->vid_mask_psi);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_mask_psi, "long_name", "mask at PSI-points");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_mask_psi, "option_0", "land");
    add_txt_attribute_netcdf(E, E->ncid, E->vid_mask_psi, "option_1", "water");
    
    
	defvar_netcdf(E, E->ncid, "mask_u", NC_DOUBLE, 2, &E->dimIdsU[0], &E->vid_mask_u);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_mask_u, "long_name", "mask at U-points");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_mask_u, "option_0", "land");
    add_txt_attribute_netcdf(E, E->ncid, E->vid_mask_u, "option_1", "water");
    
   
	defvar_netcdf(E, E->ncid, "mask_v", NC_DOUBLE, 2, &E->dimIdsV[0], &E->vid_mask_v);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_mask_v, "long_name", "mask at V-points");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_mask_v, "option_0", "land");
    add_txt_attribute_netcdf(E, E->ncid, E->vid_mask_v, "option_1", "water");
    

	defvar_netcdf(E, E->ncid, "pm", NC_DOUBLE, 2, &E->dimIdsRho[0], &E->vid_pm);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_pm, "long_name", "curvilinear coordinate metric in X");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_pm, "units", "meter-1");
    
 
	defvar_netcdf(E, E->ncid, "pn", NC_DOUBLE, 2, &E->dimIdsRho[0], &E->vid_pn);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_pn, "long_name", "curvilinear coordinate metric in ETA");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_pn, "units", "meter-1");
    
	
    
    
	defvar_netcdf(E, E->ncid, "spherical", NC_CHAR, 1, &E->dimIdsOne[0], &E->vid_spherical);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_spherical, "long_name", "grid type logical switch");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_spherical, "option_T", "spherical");
    
	defvar_netcdf(E, E->ncid, "xl", NC_DOUBLE, 1, &E->dimIdsOne[0], &E->vid_xl);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_xl, "long_name", "domain length in the XI-direction");
	add_txt_attribute_netcdf(E, E->ncid, E->vid_xl, "units", "degrees");
    
	defvar_netcdf(E, E->ncid, "X", NC_DOUBLE, 1, &E->dimIdsOne[0], &E->vid_X);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_X, "long_name", "width of the domain (degrees)");
    
	defvar_netcdf(E, E->ncid, "Y", NC_DOUBLE, 1, &E->dimIdsOne[0], &E->vid_Y);	
    add_txt_attribute_netcdf(E, E->ncid, E->vid_Y, "long_name", "length of the domain (degrees)");
    
	//defvar_netcdf(E, E->ncid, "dx", NC_DOUBLE, 1, &E->dimIdsOne[0], &E->vid_dx);	
    //add_txt_attribute_netcdf(E, E->ncid, E->vid_dx, "long_name", "resolution in x (degrees)");
    
	//defvar_netcdf(E, E->ncid, "dy", NC_DOUBLE, 1, &E->dimIdsOne[0], &E->vid_dy);	
    //add_txt_attribute_netcdf(E, E->ncid, E->vid_dy, "long_name", "resolution in y (degrees)");
    
    
}



void defvar_netcdf(e *E, int ncid, char *var_name, nc_type type, int dims, int *dimIds, int *varid){
	
	
	if((E->retval = nc_redef(ncid)))
		fail("nc_redef failed\n");
    
	if ((E->retval = nc_def_var(ncid, var_name, type, dims, dimIds, varid)))
		fail("nc_def_var failed\n");
	
	if ((E->retval = nc_enddef(ncid)))
		fail("nc_enddef failed\n");
	
}


void add_txt_attribute_netcdf(e *E, int ncid, int varid, char* att_name, char* att_value){
    
	if((E->retval = nc_redef(ncid)))
		fail("nc_redef failed\n");
    
	if(( E->retval = nc_put_att_text(ncid, varid, att_name, strlen(att_value), att_value)))
		fail("nc_put_att_txt failed\n");
	
	if ((E->retval = nc_enddef(ncid)))
		fail("nc_enddef failed\n");
}


void add_global_metadata(e *E, int ncid){
    
    // TODO!
}


void write_data(e *E){
    
    
    // write angle
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_angle, &E->angle[0][0])))
        fail("put_var_ failed for angle. Error code = %d\n",E->retval);
    
    // write dmde
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_dmde, &E->dmde[0][0])))
            fail("put_var_ failed for dmdw. Error code = %d\n",E->retval);   
    // write dndx
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_dndx, &E->dndx[0][0])))
        fail("put_var_ failed for dndx. Error code = %d\n",E->retval);
    
    // write el
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_el, &E->el)))
        fail("put_var_ failed. Error code = %d\n",E->retval);
    // write f
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_f, &E->f[0][0])))
        fail("put_var_ failed. Error code = %d\n",E->retval);
    
    // write h
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_h, &E->h[0][0])))
        fail("put_var_ failed. Error code = %d\n",E->retval);
    
    
    // lat_rho
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_lat_rho, &E->lat_rho[0][0])))
        fail("put_var_ failed for lat_rho. Error code = %d\n",E->retval);
    
    // lat_psi
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_lat_psi, &E->lat_psi[0][0])))
        fail("put_var_ failed. Error code = %d\n",E->retval);
    
    // lat_u
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_lat_u, &E->lat_u[0][0])))
        fail("put_var_ failed. Error code = %d\n",E->retval);
    
    // lat_v
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_lat_v, &E->lat_v[0][0])))
        fail("put_var_ failed. Error code = %d\n",E->retval);
    
    // lon_rho
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_lon_rho, &E->lon_rho[0][0])))
        fail("put_var_ failed. Error code = %d\n",E->retval);
    
    // lon_psi
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_lon_psi, &E->lon_psi[0][0])))
        fail("put_var_ failed. Error code = %d\n",E->retval);
    
    // lon_u
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_lon_u, &E->lon_u[0][0])))
        fail("put_var_ failed. Error code = %d\n",E->retval);
    
    // lon_v
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_lon_v, &E->lon_v[0][0])))
        fail("put_var_ failed. Error code = %d\n",E->retval);
    
    
    // mask_rho
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_mask_rho, &E->mask_rho[0][0])))
        fail("put_var_ failed. Error code = %d\n",E->retval);
    
    // mask_psi
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_mask_psi, &E->mask_psi[0][0])))
        fail("put_var_ failed. Error code = %d\n",E->retval);
    
    // mask_u
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_mask_u, &E->mask_u[0][0])))
        fail("put_var_ failed. Error code = %d\n",E->retval);
    
    // mask_v
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_mask_v, &E->mask_v[0][0])))
        fail("put_var_ failed. Error code = %d\n",E->retval);
    
    // pm
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_pm, &E->pm[0][0])))
        fail("put_var_ failed. Error code = %d\n",E->retval);
    
    // pn
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_pn, &E->pn[0][0])))
        fail("put_var_ failed. Error code = %d\n",E->retval);
    
    // spherical
    sprintf(E->spherical,"T");
    if ((E->retval = nc_put_var_text(E->ncid, E->vid_spherical, E->spherical)))
        fail("put_var_ failed. Error code = %d\n",E->retval);
    
    // xl
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_xl, &E->xl)))
        fail("put_var_ failed. Error code = %d\n",E->retval);
    
    // X
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_X, &E->g.X)))
        fail("put_var_ failed. Error code = %d\n",E->retval);
    // Y
    if ((E->retval = nc_put_var_double(E->ncid, E->vid_Y, &E->g.Y)))
        fail("put_var_ failed. Error code = %d\n",E->retval);
    // dx
    //if ((E->retval = nc_put_var_double(E->ncid, E->vid_dx, &E->dx[0][0])))
    //    fail("put_var_ failed. Error code = %d\n",E->retval);
    // dy
    //if ((E->retval = nc_put_var_double(E->ncid, E->vid_dy, &E->dy[0][0])))
    //    fail("put_var_ failed. Error code = %d\n",E->retval);
    
}
