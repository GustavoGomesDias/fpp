#ifndef __infected_h__
#define __infected_h__
#include "resource.h"

typedef struct infected infected;

infected *make_infected(resource *infinite_res);

void add_necessary_res(infected *this, resource *new_necessary_res);

void add_my_res(infected *this, resource *new_res);

resource *get_necessary_res(infected *this);

int get_infected_cycle_count(infected *this);

void make_solution(infected *this);

#endif