#ifndef __lab_h__
#define __lab_h__
#include "resource.h"

typedef struct lab lab;

lab *create_lab(char first_res[120], char second_res[120]);

resource *make_first_resource(lab *this);

resource *make_second_resource(lab *this);

resource **make_all_resources(lab *this);

int get_infected_cycle_count(lab *this);

#endif