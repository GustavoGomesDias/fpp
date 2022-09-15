#include <stdio.h>
#include <stdlib.h>
#include "lab.h"

struct lab {
  int cycle_count; // number of all resources produced
  char first_res[120];
  char second_res[120];
};

lab *create_lab(char first_res[120], char second_res[120]) {
  lab *new_lab = (lab *) malloc(sizeof(lab));
  strcpy(new_lab->first_res, first_res);
  strcpy(new_lab->second_res, second_res);
  new_lab->cycle_count = 0;

}

resource *make_first_resource(lab *this) {
  return create_resource(this->first_res, this);
}

resource *make_second_resource(lab *this) {
  return create_resource(this->second_res, this);
}

resource **make_all_resources(lab *this) {
  resource **res_list;
  res_list[0] = make_first_resource(this);
  res_list[1] = make_second_resource(this);
  this->cycle_count += 1;
  return res_list;
}

int get_infected_cycle_count(lab *this) {
  return this->cycle_count;
}
