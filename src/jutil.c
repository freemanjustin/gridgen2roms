// jutils
//
// freeman.justin@gmail.com



#include "jutil.h"

void get_command_line_arg_as_string(char **variable, char *cli_input){
    
	*variable = malloc((strlen(cli_input)+1)*sizeof(char));
	strncpy(*variable, cli_input, strlen(cli_input));
	// fix the string
	*(&(*variable)[strlen(cli_input)]) = '\x0';
}

void get_command_line_arg_as_int(int *variable, char *cli_input){
	
	sscanf(cli_input,"%d",variable);
}

double ****malloc4d_double(int dim1, int dim2, int dim3, int dim4)
{
	size_t		layer1_count = dim1;
	size_t		layer2_count = dim1 * dim2;
	size_t		layer3_count = dim1 * dim2 * dim3;
	size_t		layer1_size = sizeof(double ***) * layer1_count;
	size_t		layer2_size = sizeof(double **) * layer2_count;
	size_t	 	layer3_size = sizeof(double *) * layer3_count;
	size_t 	layers_size = layer1_size + layer2_size + layer3_size;
    
	size_t	 	data_count = dim1 * dim2 * dim3 * dim4;
	size_t 	data_size = sizeof(double) * data_count;
    
	void 	*raw_bytes = (void*)malloc(layers_size + data_size);
    
	double ****layer1 = (double ****)(raw_bytes);
	double ***layer2 = (double ***)(raw_bytes + layer1_size);
	double **layer3 = (double **)(raw_bytes + layer1_size + layer2_size);
    
	double *double_data = (double *)(raw_bytes + layers_size);
    
	int i, j, k;
	double ***this_layer2;
	double **this_layer3;
    
	for (i = 0; i < dim1; i++) {
		layer1[i] = layer2 + (i * dim2);
		this_layer2 = layer1[i];
        
		for (j = 0; j < dim2; j++) {
			this_layer2[j] = layer3 + (i * dim2 * dim3 + j * dim3);
			this_layer3 = this_layer2[j];
            
			for (k = 0; k < dim3; k++) {
				this_layer3[k] = double_data + (i * dim2 * dim3 * dim4 + j * dim3 * dim4 + k * dim4);
			}
		}
	}
	return layer1;
}

double ***malloc3d_double(int dim1, int dim2, int dim3)
{
    
	size_t		layer1_count = dim1;
	size_t		layer2_count = dim1 * dim2;
	size_t		layer1_size = sizeof(double ***) * layer1_count;
	size_t		layer2_size = sizeof(double **) * layer2_count;
    
	size_t 	layers_size = layer1_size + layer2_size ;
    
	size_t	 	data_count = dim1 * dim2 * dim3 ;
	size_t 	data_size = sizeof(double) * data_count;
    
	void 	*raw_bytes = (void*)malloc(layers_size + data_size);
    
	double ***layer1 = (double ***)(raw_bytes);
	double **layer2 = (double **)(raw_bytes + layer1_size);
	double *double_data = (double *)(raw_bytes + layers_size);
    
	int i, j;
	double **this_layer2;
	//double *this_layer3;
    
	for (i = 0; i < dim1; i++) {
		layer1[i] = layer2 + (i * dim2);
		this_layer2 = layer1[i];
        
		for (j = 0; j < dim2; j++) {
			this_layer2[j] = double_data + (i * dim2 * dim3 + j * dim3);
		}
	}
	return layer1;
    
}


float ***malloc3d_float(int dim1, int dim2, int dim3)
{
    
	size_t		layer1_count = dim1;
	size_t		layer2_count = dim1 * dim2;
	size_t		layer1_size = sizeof(float ***) * layer1_count;
	size_t		layer2_size = sizeof(float **) * layer2_count;
    
	size_t 	layers_size = layer1_size + layer2_size ;
    
	size_t	 	data_count = dim1 * dim2 * dim3 ;
	size_t 	data_size = sizeof(float) * data_count;
    
	void 	*raw_bytes = (void*)malloc(layers_size + data_size);
    
	float ***layer1 = (float ***)(raw_bytes);
	float **layer2 = (float **)(raw_bytes + layer1_size);
	float *double_data = (float *)(raw_bytes + layers_size);
    
	int i, j;
	float **this_layer2;
	//double *this_layer3;
    
	for (i = 0; i < dim1; i++) {
		layer1[i] = layer2 + (i * dim2);
		this_layer2 = layer1[i];
        
		for (j = 0; j < dim2; j++) {
			this_layer2[j] = double_data + (i * dim2 * dim3 + j * dim3);
		}
	}
	return layer1;
    
}


int ***malloc3d_int(int dim1, int dim2, int dim3)
{
    
	size_t		layer1_count = dim1;
	size_t		layer2_count = dim1 * dim2;
	size_t		layer1_size = sizeof(int ***) * layer1_count;
	size_t		layer2_size = sizeof(int **) * layer2_count;
    
	size_t 	layers_size = layer1_size + layer2_size ;
    
	size_t	 	data_count = dim1 * dim2 * dim3 ;
	size_t 	data_size = sizeof(int) * data_count;
    
	void 	*raw_bytes = (void*)malloc(layers_size + data_size);
    
	int ***layer1 = (int ***)(raw_bytes);
	int **layer2 = (int **)(raw_bytes + layer1_size);
	int *double_data = (int *)(raw_bytes + layers_size);
    
	int i, j;
	int **this_layer2;
	//double *this_layer3;
    
	for (i = 0; i < dim1; i++) {
		layer1[i] = layer2 + (i * dim2);
		this_layer2 = layer1[i];
        
		for (j = 0; j < dim2; j++) {
			this_layer2[j] = double_data + (i * dim2 * dim3 + j * dim3);
		}
	}
	return layer1;
    
}


short int ***malloc3d_short(int dim1, int dim2, int dim3)
{
    
	size_t		layer1_count = dim1;
	size_t		layer2_count = dim1 * dim2;
	size_t		layer1_size = sizeof(short int ***) * layer1_count;
	size_t		layer2_size = sizeof(short int **) * layer2_count;
    
	size_t 	layers_size = layer1_size + layer2_size ;
    
	size_t	 	data_count = dim1 * dim2 * dim3 ;
	size_t 	data_size = sizeof(short int) * data_count;
    
	void 	*raw_bytes = (void*)malloc(layers_size + data_size);
    
	short int ***layer1 = (short int ***)(raw_bytes);
	short int **layer2 = (short int **)(raw_bytes + layer1_size);
	short int *double_data = (short int *)(raw_bytes + layers_size);
    
	int i, j;
	short int **this_layer2;
	//double *this_layer3;
    
	for (i = 0; i < dim1; i++) {
		layer1[i] = layer2 + (i * dim2);
		this_layer2 = layer1[i];
        
		for (j = 0; j < dim2; j++) {
			this_layer2[j] = double_data + (i * dim2 * dim3 + j * dim3);
		}
	}
	return layer1;
    
}


signed char ***malloc3d_char(int dim1, int dim2, int dim3)
{
    
	size_t		layer1_count = dim1;
	size_t		layer2_count = dim1 * dim2;
	size_t		layer1_size = sizeof(signed char ***) * layer1_count;
	size_t		layer2_size = sizeof(signed char **) * layer2_count;
    
	size_t 	layers_size = layer1_size + layer2_size ;
    
	size_t	 	data_count = dim1 * dim2 * dim3 ;
	size_t 	data_size = sizeof(signed char) * data_count;
    
	void 	*raw_bytes = (void*)malloc(layers_size + data_size);
    
	signed char ***layer1 = (signed char ***)(raw_bytes);
	signed char **layer2 = (signed char **)(raw_bytes + layer1_size);
	signed char *double_data = (signed char *)(raw_bytes + layers_size);
    
	int i, j;
	signed char **this_layer2;
	//double *this_layer3;
    
	for (i = 0; i < dim1; i++) {
		layer1[i] = layer2 + (i * dim2);
		this_layer2 = layer1[i];
        
		for (j = 0; j < dim2; j++) {
			this_layer2[j] = double_data + (i * dim2 * dim3 + j * dim3);
		}
	}
	return layer1;
    
}




double **malloc2d_double(int dim1, int dim2)
{
    int i;
    double **p, *base;
    
    // Allocate array of pointers and 1D block of doubles
    p = (double **)malloc(dim1 * sizeof(double *));
    base = (double *)malloc(dim1 * dim2 * sizeof(double));
    if (p == NULL || base == NULL)
        return NULL;
    
    // Set pointers for each row
    for (i = 0; i < dim1; i++) {
        p[i] = &base[dim2 * i];
    }
    
    return p;
    
}

int **malloc2d_int(int dim1, int dim2) {
    
    int i;
    int **p, *base;
    
    // Allocate array of pointers and 1D block of integers
    p = (int **)malloc(dim1 * sizeof(int *));
    base = (int *)malloc(dim1 * dim2 * sizeof(int));
    if (p == NULL || base == NULL)
        return NULL;
    
    // Set pointers for each row
    for (i = 0; i < dim1; i++) {
        p[i] = &base[dim2 * i];
    }
    
    return p;
    
}

float **malloc2d_float(int dim1, int dim2)
{
    int i;
    float **p, *base;
    
    // Allocate array of pointers and 1D block of floats
    p = (float **)malloc(dim1 * sizeof(float *));
    base = (float *)malloc(dim1 * dim2 * sizeof(float));
    if (p == NULL || base == NULL)
        return NULL;
    
    // Set pointers for each row
    for (i = 0; i < dim1; i++) {
        p[i] = &base[dim2 * i];
    }
    
    return p;
    
}

signed char **malloc2d_signedchar(int dim1, int dim2)
{
    int i;
    signed char **p, *base;
    
    // Allocate array of pointers and 1D block of signed chars
    p = (signed char **)malloc(dim1 * sizeof(signed char *));
    base = (signed char *)malloc(dim1 * dim2 * sizeof(signed char));
    if (p == NULL || base == NULL)
        return NULL;
    
    // Set pointers for each row
    for (i = 0; i < dim1; i++) {
        p[i] = &base[dim2 * i];
    }
    
    return p;
    
}

char **malloc2d_char(int dim1, int dim2)
{
    int i;
    char **p, *base;
    
    // Allocate array of pointers and 1D block of chars
    p = ( char **)malloc(dim1 * sizeof( char *));
    base = ( char *)malloc(dim1 * dim2 * sizeof( char));
    if (p == NULL || base == NULL)
        return NULL;
    
    // Set pointers for each row
    for (i = 0; i < dim1; i++) {
        p[i] = &base[dim2 * i];
    }
    
    return p;
    
}

short int **malloc2d_short(int dim1, int dim2)
{
    int i;
    short int **p, *base;
    
    // Allocate array of pointers and 1D block of short integers
    p = (short int **)malloc(dim1 * sizeof(short int *));
    base = (short int *)malloc(dim1 * dim2 * sizeof(short int));
    if (p == NULL || base == NULL)
        return NULL;
    
    // Set pointers for each row
    for (i = 0; i < dim1; i++) {
        p[i] = &base[dim2 * i];
    }
    
    return p;
    
}

// Left justify and right pack a string

char *lr_pack( char *c ){
	int j ;
    
	if(c == NULL) return c;
    
	// Left pack
	while(strlen(c)>0 && *c==' ')
	    for(j=0;j<(int)strlen(c);j++) *(c+j) = *(c+j+1);
    
	// get rid of tabs
	while( strlen(c)>0 && *c=='\t' )
	    for( j=0 ; j<(int)strlen(c) ; j++ ) *(c+j) = *(c+j+1) ;
    
	// Get rid of blanks on the right
	while(strlen(c)>0 && *(c+strlen(c)-1)==' ') *(c+strlen(c)-1) = '\x0';
    
	// Get rid of tabs on right
    while(strlen(c)>0 && *(c+strlen(c)-1)=='\t') *(c+strlen(c)-1) = '\x0';
    
	return(c);
}

char *strip_slash_n( char *c ){
	
	if(c == NULL) return c;
	
	// get rid of new lines
	while(strlen(c)>0 && *(c+strlen(c)-1)=='\n') *(c+strlen(c)-1) = '\x0';
	
	return(c);
}

// rounds a double to the nearest integer
// called round_double to avoid potential
// namespace clashes with the c99 function round()
double round_double(double x) {
    
    return fabs(x) < 0.0 ? ceil(x-0.5) : floor(x+0.5);
}


// modulo function
// works for negative numbers
// the math.h fmod() is undefined for negatives
// input arguments are doubles and the function returns a double
double j_fmod(double x, double y) {
    
    return fmod((y + fmod(x,y)), y);
}

void julday(int mm, int dd, int yy, int hh, int mn, double sec, double *JUL){
	
	// Gregorian Calander was adopted on Oct. 15, 1582
	// skipping from Oct. 4, 1582 to Oct. 15, 1582
	long int GREG = 2299171; // incorrect Julian day for Oct. 25, 1582
	
	int bc, inJanFeb, JY, JM;
	long int JA;
	
	// bc?
	if (yy < 0) 
		bc = 1;
	else 
		bc = 0;
	
	yy = yy + bc;
	
	//	#are we in Jan or Feb (mm <= 2)
	if (mm <= 2) 
		inJanFeb = 1;
	else
		inJanFeb = 0;
	
	JY = yy - inJanFeb;
	//	# JM ranges from 3(Mar)-12(Dec),13(Jan),14(Feb)
	JM = mm + (1 + 12*inJanFeb);
	//	# calc julian date...
	*JUL = (floor(365.25 * JY) + floor(30.6001*JM) + dd + 1720995);
	//	#do we really need a gregorian date?
	if (*JUL >= GREG){
		JA = floor(0.01 * JY);
		*JUL = *JUL + 2 - JA + (int)floor(0.25 * JA);
	}
	//	# add the time component
	*JUL = *JUL + ((double)hh/24.0 - 0.5) + ((double)mn/1440.0) + ((double)sec/86400.);
	
}


void caldat(double JUL, int *mm, int *dd, int *yy, int *hh, int *mn, double *sec){
	
	long int igreg, julLong, jalpha, ja, jb, jc, jd, je;
	double 	fractime;
	
	igreg = 2299161;
	
	julLong = (long)(JUL+0.5);
	
	if (julLong >= igreg){
		jalpha = (int)( ((julLong - 1867216) - 0.25) / 36524.2);
        ja = julLong + 1 + jalpha - (int)(0.25 * jalpha);
	}
	else
        ja = julLong;
	
	jb = ja + 1524;
	jc = floor((6680 + ((jb-2439870)-122.1)/365.25));
	jd = floor((365 * jc + (0.25 * jc)));
	je = floor(((jb - jd) / 30.6001));
	
	*dd = (int)(floor(jb - jd - (int)(30.6001 * je)));
	*mm = (int)(je - 1);
	*mm = (int)(((*mm - 1) % 12) + 1);
	*yy = (int)(jc - 4715);
	
	if (*mm > 2) 
		*yy = *yy - 1;
	
	if (*mm <= 0) 
		*yy = *yy - 1;
	
	
	fractime = (JUL + 0.5) - (double)julLong;
	*hh = (int)(fractime * 24.0);
	fractime = fractime - (double)*hh/24.0;
	*mn =(int)(fractime*1440.0);
	*sec = (fractime - (double)*mn/1440.0) * 86400.0;
	
}


