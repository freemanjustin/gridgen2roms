// gridr
//
// freeman.justin@gmail.com



#include "grid.h"


#define ERRCODE 2
#define ERR(e) {printf("Error: %s\nFunction: %s\nFile: %s\nLine %d\n", nc_strerror(e),__func__,__FILE__,__LINE__); exit(ERRCODE);}

void get_params(e *E){

	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(E->input_xml);

	if (doc == NULL ) {
		fail("Could not open input file %s\n", E->input_xml);
	}

	cur = xmlDocGetRootElement(doc);

	if (cur == NULL) {
		xmlFreeDoc(doc);
		fail("empty input file %s\n", E->input_xml);
	}

	if (xmlStrcmp(cur->name, (const xmlChar *) "grid")) {
		xmlFreeDoc(doc);
		fail("%s is the wrong type or not a valid input xml file.\nroot node != <grid>\n", E->input_xml);
	}

	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *) "params"))){
			_parseInputFile_params (E, doc, cur);
		}
        else if((!xmlStrcmp(cur->name, (const xmlChar *) "bathymetry"))){
			_parseInputFile_bathymetry (E, doc, cur);
		}
		cur = cur->next;
	}

	xmlFreeDoc(doc);

}


/**************************************************************************************
 *
 * 	private functions
 *
 **************************************************************************************/

void _parseInputFile_params (e *E, xmlDocPtr doc, xmlNodePtr cur) {

    xmlChar *key;

	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if((!xmlStrcmp(cur->name, (const xmlChar *) "corner_lat"))){
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            lr_pack( (char *)key );
            sscanf((char*)key,"%lf", &E->g.lat);
            xmlFree(key);
		}
		else if((!xmlStrcmp(cur->name, (const xmlChar *) "corner_lon"))){
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            lr_pack( (char *)key );
            sscanf((char*)key,"%lf", &E->g.lon);
            xmlFree(key);
		}
		else if((!xmlStrcmp(cur->name, (const xmlChar *) "width"))){
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            lr_pack( (char *)key );
            sscanf((char*)key,"%lf", &E->g.X);
            xmlFree(key);
		}
        else if((!xmlStrcmp(cur->name, (const xmlChar *) "length"))){
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            lr_pack( (char *)key );
            sscanf((char*)key,"%lf", &E->g.Y);
            xmlFree(key);
		}
        else if((!xmlStrcmp(cur->name, (const xmlChar *) "angle"))){
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            lr_pack( (char *)key );
            sscanf((char*)key,"%lf", &E->g.rotangle);
            xmlFree(key);
		}
        else if((!xmlStrcmp(cur->name, (const xmlChar *) "resolution"))){
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            lr_pack( (char *)key );
            sscanf((char*)key,"%lf", &E->g.resol);
            xmlFree(key);
		}
        else if((!xmlStrcmp(cur->name, (const xmlChar *) "vertical_levels"))){
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            lr_pack( (char *)key );
            sscanf((char*)key,"%d", &E->g.N);
            xmlFree(key);
		}
        cur = cur->next;
	}

	// error check


}


void _parseInputFile_bathymetry(e *E, xmlDocPtr doc, xmlNodePtr cur){

	xmlChar *key;
    int retval;
    int varid;
    int ncid;

    cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if((!xmlStrcmp(cur->name, (const xmlChar *) "filename"))){// the field variable names
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            lr_pack( (char *)key );
            E->b.fname = (char*)malloc((strlen((char*)key)+1)*sizeof(char));
            strncpy(&E->b.fname[0], (char*)key, strlen((char*)key));
            *(&E->b.fname[strlen((char*)key)]) = '\x0';
            xmlFree(key);
		}
        else if((!xmlStrcmp(cur->name, (const xmlChar *) "lat_coord"))){// the field variable names
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            lr_pack( (char *)key );
            E->b.lat_name = (char*)malloc((strlen((char*)key)+1)*sizeof(char));
            strncpy(&E->b.lat_name[0], (char*)key, strlen((char*)key));
            *(&E->b.lat_name[strlen((char*)key)]) = '\x0';
            xmlFree(key);
		}
        else if((!xmlStrcmp(cur->name, (const xmlChar *) "lon_coord"))){// the field variable names
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            lr_pack( (char *)key );
            E->b.lon_name = (char*)malloc((strlen((char*)key)+1)*sizeof(char));
            strncpy(&E->b.lon_name[0], (char*)key, strlen((char*)key));
            *(&E->b.lon_name[strlen((char*)key)]) = '\x0';
            xmlFree(key);
		}
        else if((!xmlStrcmp(cur->name, (const xmlChar *) "field"))){// the field variable names
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            lr_pack( (char *)key );
            E->b.field_name = (char*)malloc((strlen((char*)key)+1)*sizeof(char));
            strncpy(&E->b.field_name[0], (char*)key, strlen((char*)key));
            *(&E->b.field_name[strlen((char*)key)]) = '\x0';
            xmlFree(key);
		}
        else if((!xmlStrcmp(cur->name, (const xmlChar *) "min_depth"))){// the field variable names
            key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            lr_pack( (char *)key );
            sscanf((char*)key,"%lf", &E->b.min_depth);
            xmlFree(key);

		}

        cur = cur->next;
	}

    // got the file info, lets read it into memory

    // open the file
    if((retval = nc_open(E->b.fname, NC_NOWRITE, &ncid)))
        ERR(retval);

    // get the lat dimension sizes
    if((retval = nc_inq_dimid(ncid, E->b.lat_name, &varid)))
        ERR(retval);

    if((retval = nc_inq_dimlen(ncid,varid,&E->b.nlat)))
        ERR(retval);


    // get the lon dimension sizes
    if((retval = nc_inq_dimid(ncid, E->b.lon_name, &varid)))
        ERR(retval);

    if((retval = nc_inq_dimlen(ncid,varid,&E->b.nlon)))
        ERR(retval);


    // malloc room for the arrays
    E->b.lat = malloc(E->b.nlat*sizeof(double));
    E->b.lon = malloc(E->b.nlon*sizeof(double));
    E->b.field = malloc2d_double(E->b.nlat, E->b.nlon);

    // read the data
    nc_inq_varid(ncid, E->b.lat_name, &varid);
    nc_get_var_double(ncid, varid, E->b.lat);

    nc_inq_varid(ncid, E->b.lon_name, &varid);
    nc_get_var_double(ncid, varid, E->b.lon);

    nc_inq_varid(ncid, E->b.field_name, &varid);
    nc_get_var_double(ncid, varid, &E->b.field[0][0]);

	// error check
    // TODO!
}
