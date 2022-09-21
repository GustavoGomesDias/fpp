#ifndef __lab_h__
#define __lab_h__
#include "resource.h"

typedef struct lab lab;

lab *create_lab(char first_res[], char second_res[], int id);

resource *make_first_resource(lab *this);
resource *make_second_resource(lab *this);
resource **make_all_resources(lab *this);

int get_lab_cycle_count(lab *this);
void set_lab_cycle_count(lab *this);

int get_laboratory_id(lab *this);

#endif