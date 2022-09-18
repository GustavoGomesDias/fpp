#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab.h"

struct lab {
  int id;
  int cycle_count; // number of all resources produced
  char *first_res;
  char *second_res;
};

lab *create_lab(char first_res[], char second_res[], int id) {
  lab *new_lab = (lab *) malloc(sizeof(lab));
  
  new_lab->first_res = first_res;
  new_lab->second_res = second_res;
  
  new_lab->cycle_count = 0;
  new_lab->id = id;
  return new_lab;  
}

resource *make_first_resource(lab *this) {
  return create_resource(this->first_res, this->id);
}

resource *make_second_resource(lab *this) {
  return create_resource(this->second_res, this->id);
}

resource **make_all_resources(lab *this) {
  resource **res_list = malloc(sizeof(resource *) * 2);
  res_list[0] = make_first_resource(this);
  res_list[1] = make_second_resource(this);
  this->cycle_count += 1;
  return res_list;
}

int get_lab_cycle_count(lab *this) {
  return this->cycle_count;
}

void set_lab_cycle_count(lab *this) {
  this->cycle_count += 1;
}

int get_lab_id(lab *this) {
  return this->id;
}
