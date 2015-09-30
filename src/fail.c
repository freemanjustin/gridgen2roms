// gridr
//
// freeman.justin@gmail.com

#include "grid.h"


void grid_fail( const int line, const char *func, const char *file, const char *msg, ... ) {
    va_list args;
    
    printf("error: file %s in function %s at line %d \n", file, func, line );
    
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);
    
	exit(1);
    
}