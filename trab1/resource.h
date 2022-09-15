#ifndef __resource_h__
#define __resource_h__
#include "lab.h"

typedef struct resource resource;

resource *create_resource(char res_name[120], lab *create_lab);

lab *get_lab(resource *this);

resource *get_next_res(resource *res);

char *get_name(resource *this);

void set_next_res(resource *this, resource *next_res);

#endif