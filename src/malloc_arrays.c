// gridr
//
// freeman.justin@gmail.com


#include "grid.h"

void malloc_arrays(e *E){

    E->x = malloc(E->g.nX * sizeof(double));
    E->y = malloc((E->g.nY+1) * sizeof(double));
    
    E->x_rho = malloc2d_double(E->g.nY+1, E->g.nX);
    E->y_rho = malloc2d_double(E->g.nY+1, E->g.nX);
    
    E->Rx_rho = malloc2d_double(E->g.nY+1, E->g.nX);
    E->Ry_rho = malloc2d_double(E->g.nY+1, E->g.nX);
    
    E->lat_rho = malloc2d_double(E->g.nY+1, E->g.nX);
    E->lon_rho = malloc2d_double(E->g.nY+1, E->g.nX);
    
    E->x_u = malloc2d_double(E->g.nY, E->g.nX-1);
    E->y_u = malloc2d_double(E->g.nY, E->g.nX-1);

    E->Rx_u = malloc2d_double(E->g.nY, E->g.nX-1);
    E->Ry_u = malloc2d_double(E->g.nY, E->g.nX-1);
    
    E->lat_u = malloc2d_double(E->g.nY, E->g.nX-1);
    E->lon_u = malloc2d_double(E->g.nY, E->g.nX-1);
    
    E->x_v = malloc2d_double(E->g.nY-1, E->g.nX);
    E->y_v = malloc2d_double(E->g.nY-1, E->g.nX);

    E->Rx_v = malloc2d_double(E->g.nY-1, E->g.nX);
    E->Ry_v = malloc2d_double(E->g.nY-1, E->g.nX);
    
    E->lat_v = malloc2d_double(E->g.nY-1, E->g.nX);
    E->lon_v = malloc2d_double(E->g.nY-1, E->g.nX);
    
    E->x_psi = malloc2d_double(E->g.nY-1, E->g.nX-1);
    E->y_psi = malloc2d_double(E->g.nY-1, E->g.nX-1);
    
    E->Rx_psi = malloc2d_double(E->g.nY-1, E->g.nX-1);
    E->Ry_psi = malloc2d_double(E->g.nY-1, E->g.nX-1);
    
    E->lat_psi = malloc2d_double(E->g.nY-1, E->g.nX-1);
    E->lon_psi = malloc2d_double(E->g.nY-1, E->g.nX-1);
    
    E->dx = malloc2d_double(E->g.nY, E->g.nX);
    E->dy = malloc2d_double(E->g.nY, E->g.nX);
    
    E->pm = malloc2d_double(E->g.nY, E->g.nX);
    E->pn = malloc2d_double(E->g.nY, E->g.nX);

    E->dndx =  malloc2d_double(E->g.nY, E->g.nX);
    E->dmde =  malloc2d_double(E->g.nY, E->g.nX);
    
    E->f =  malloc2d_double(E->g.nY, E->g.nX);
    E->h =  malloc2d_double(E->g.nY, E->g.nX);
    
    E->mask_rho = malloc2d_double(E->g.nY, E->g.nX);
    E->mask_u = malloc2d_double(E->g.nY, E->g.nX-1);
    E->mask_v = malloc2d_double(E->g.nY-1, E->g.nX);
    E->mask_psi = malloc2d_double(E->g.nY-1, E->g.nX-1);
    
    
    E->angle = malloc2d_double(E->g.nY, E->g.nX);
    
}
