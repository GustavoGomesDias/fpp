#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "infected.h"

struct infected {
  int necessary_res_size;
  int my_res_size;
  int cycle_count; // number of vaccines produced

  char *infinite_res;
  char *necessary_res[2];
  resource **my_res;
};

infected *create_infected(char infinite_res[]) {
  infected *new_infected = (infected *) malloc(sizeof(infected));
  // new_infected->infinite_res = malloc(sizeof(infinite_res));
  // strcpy(new_infected->infinite_res, infinite_res);

  new_infected->infinite_res = infinite_res;
  new_infected->my_res = malloc(sizeof(resource *) * 2);
  new_infected->necessary_res_size = 0;
  new_infected->my_res_size = 0;
  new_infected->cycle_count = 0;

  return new_infected;
}

void add_necessary_res(infected *this, char new_necessary_res[]) {
  if (this->necessary_res_size == 2) {
    return;
  }

  if (this->necessary_res[0] == NULL) {
    this->necessary_res_size += 1;
    this->necessary_res[0] = new_necessary_res;
    return;
  }

  this->necessary_res[1] = new_necessary_res;
  this->necessary_res_size += 1;
}

void add_my_res(infected *this, resource *new_res) {
  if (this->my_res_size == 2) {
    return;
  }

  if (this->necessary_res[0] == get_name(new_res) || this->necessary_res[1] == get_name(new_res)) {
    this->my_res_size += 1;
    if (this->my_res[0] == NULL) {
      this->my_res[0] = new_res;
      return;
    }

    set_next_res(this->my_res[0], new_res);
  }
}

char *get_necessary_res(infected *this, int position) {
  return this->necessary_res[position];
}

int get_infected_cycle_count(infected *this) {
  return this->cycle_count;
}

void set_infected_cycle_count(infected *this) {
  this->cycle_count += 1;
}

void make_solution(infected *this) {
  if (this->my_res_size == 2) {
    this->cycle_count += 1;
    this->my_res_size = 0;
  }

  return;
}

char *get_inf_res(infected *this) {
  return this->infinite_res;
}
