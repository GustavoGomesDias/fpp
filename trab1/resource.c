#include <stdio.h>
#include <stdlib.h>
#include "resource.h"

struct resource {
  char name[120];
  int list_len;
  int actual_len;
  lab *lab;
  struct resource *next;
};

resource *create_resource(char res_name[120], lab *create_lab, int list_len) {
  resource *new_res = (resource *) malloc(sizeof(resource));

  strcpy(new_res->name, res_name);
  new_res->lab = create_lab;
  new_res->list_len = list_len;
  new_res->actual_len = 0;

  return new_res;
}

void set_next_resource(resource *this, resource *res) {
  if (this->actual_len < this->list_len) {
    this->next = res;
    this->actual_len++;
  }
}

lab *get_lab(resource *this) {
  return this->lab;
}

resource *get_next_res(resource *res) {
  return res->next;
}
