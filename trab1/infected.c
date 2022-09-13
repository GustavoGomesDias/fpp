#include <stdio.h>
#include <stdlib.h>
#include "infected.h"


struct infected {
  int num_production;
  int necessary_res_size;
  int my_res_size;
  resource *infinite_res;
  resource **necessary_res;
  resource **my_res;
};


infected *make_infected(resource *infinite_res) {
  infected *new_infected = (infected *) malloc(sizeof(infected));

  new_infected->infinite_res = infinite_res;
  new_infected->necessary_res = NULL;
  new_infected->my_res = NULL;
  new_infected->num_production = 0;
  new_infected->necessary_res_size = 0;
  new_infected->my_res_size = 0;

  return new_infected;
}


void add_necessary_res(infected *this, resource *new_necessary_res) {
  if (this->necessary_res_size == 2) {
    return;
  }

  if (this->necessary_res[0] == NULL) {
    this->necessary_res_size += 1;
    this->necessary_res[0] = new_necessary_res;
    return;
  }

  set_next_res(this->necessary_res[0], new_necessary_res);
  this->necessary_res_size += 1;
}

void add_my_res(infected *this, resource *new_res) {
  if (this->my_res_size == 2) {
    return;
  }

  if (this->necessary_res[0] == new_res || get_next_res(this->necessary_res[0]) == new_res) {
    this->my_res_size += 1;
    if (this->my_res[0] == NULL) {
      this->my_res[0] = new_res;
      return;
    }

    set_next_res(this->my_res[0], new_res);
  }
}

resource *get_necessary_res(infected *this) {
  return this->necessary_res[0];
}
