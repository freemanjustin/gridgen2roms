// gridr
//
// freeman.justin@gmail.com



#include "grid.h"
#include <complex.h>

// determine the distance on the earth between 2 points

double spheriq_dist(double lon1, double lat1, double lon2, double lat2, int debug){

    double  delta;
    double  l;
    double  beta1, beta2;
    
    double  st;

    
    if(debug == 1)
        printf("lon1 = %f, lat1 = %f, lon2 = %f, lat2 = %f\n", lon1, lat1, lon2, lat2);
    
    // Determine proper longitudinal shift.

    
    delta=lon2-lon1; 
    if(debug == 1)
        printf("\tdelta = %g\n", delta);
    
    l=fabs(delta);
    if(debug == 1)
        printf("\t(pre) l = %g", l);
    
    
    if(l>=180.0)
        l=360.0-l;
    
    if(debug == 1)
        printf("\t(post) l = %g\n", l);
    
    //  Convert Decimal degrees to radians.

    beta1 = lat1*deg2rad;
    beta2 = lat2*deg2rad;
    if(debug == 1)
        printf("beta1 = %f, beta2 = %f\n", beta1, beta2);
    
    
    l = l*deg2rad;
    if(debug == 1)
        printf("l (radians) = %g\n", l);

    // Calculate S/Bo subformulas.

    st = sqrt(( pow( (sin(l)*cos(beta2)),2.0 ) )+( pow( ( (sin(beta2)*cos(beta1) )-( sin(beta1)*cos(beta2)*cos(l)) ),2.0 ) ));

    if(debug == 1)
        printf("st = %g\n", st);
    
    // Calculate distance from point 1 to point 2

    if(debug == 1)
        printf("dist = %f\n", asin(st) * earthradius );
    
    return (asin(st) * earthradius);

}


// calculate the distance between two points on the earth
double distance(double lat1, double lon1, double lat2, double lon2){
    
	double delta_lon, delta_lat;
	double distance, angle, c;
    
    
	lat1 *= M_PI/180.0;
	lon1 *= M_PI/180.0;
	lat2 *= M_PI/180.0;
	lon2 *= M_PI/180.0;
    
	delta_lat = lat1 - lat2;
	delta_lon = fabs(lon1 - lon2);
	if (delta_lon > 2.0*M_PI) delta_lon = (2.0*M_PI) - delta_lon;
	
	angle = sin( 0.5*delta_lat)*sin(0.5*delta_lat)
    + cos(lat1)*cos(lat2)*sin(0.5*delta_lon)*sin(0.5*delta_lon);
        
    printf("angle = %f (in deg: %f)\n", angle, angle*180.0/M_PI);
	c = 2.0*asin(min(1,sqrt(angle)));
    
	distance = 6378.1*c ;
	return(distance);
}


double bearing(double lat1, double lon1, double lat2, double lon2){
    
    double delta_lon, delta_lat;
	double dep, angle;
    
    double complex z;
    
    lat1 *= M_PI/180.0;
	lon1 *= M_PI/180.0;
	lat2 *= M_PI/180.0;
	lon2 *= M_PI/180.0;
    
    
    delta_lat = lat1 - lat2;
	delta_lon = fabs(lon1 - lon2);
	if (delta_lon > 2.0*M_PI) delta_lon = (2.0*M_PI) - delta_lon;
    
    dep    = cos( 0.5*(lat2+lat1) ) * delta_lon;
    z = dep+delta_lat*I;
    angle = atan2(cimag(z),creal(z));
    
    return(angle);
  
}
