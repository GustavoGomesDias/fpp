#include <stdio.h>
#include <stdlib.h>
#include "resource.h"

struct resource {
  char name[120];
  lab *lab;
  struct resource *next;
};

resource *create_resource(char res_name[120], lab *create_lab) {
  resource *new_res = (resource *) malloc(sizeof(resource));

  strcpy(new_res->name, res_name);
  new_res->lab = create_lab;
  new_res->next = NULL;

  return new_res;
}

lab *get_lab(resource *this) {
  return this->lab;
}

resource *get_next_res(resource *res) {
  return res->next;
}

void set_next_res(resource *this, resource *next_res) {
  this->next = next_res;
}

char *get_name(resource *this) {
  return this->name;
}
