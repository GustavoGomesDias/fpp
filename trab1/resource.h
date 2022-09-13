#ifndef __resource_h__
#define __resource_h__
#include "lab.h"

typedef struct resource resource;

resource *get_next_res(resource *res);
void set_next_res(resource *this, resource *next_res);

#endif