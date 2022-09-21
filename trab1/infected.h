#ifndef __infected_h__
#define __infected_h__
#include "resource.h"

typedef struct infected infected;

infected *create_infected(char infinite_res[]);

void add_necessary_res(infected *this, char new_necessary_res[]);

void add_my_res(infected *this, resource *new_res);

char *get_necessary_res(infected *this, int position);

int get_infected_cycle_count(infected *this);
void set_infected_cycle_count(infected *this);

void make_solution(infected *this);

char *get_inf_res(infected *this);

#endif