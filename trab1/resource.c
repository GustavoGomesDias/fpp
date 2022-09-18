#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resource.h"

struct resource {
  char *name;
  int lab_id;
  struct resource *next;
};

resource *create_resource(char res_name[], int create_lab) {
  resource *new_res = (resource *) malloc(sizeof(resource));

  // new_res->name = malloc(sizeof(res_name));
  // strcpy(new_res->name, res_name);

  new_res->name = res_name;
  new_res->lab_id = create_lab;
  new_res->next = NULL;

  return new_res;
}

int get_lab_id(resource *this) {
  return this->lab_id;
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

void delete_res(resource *this) {
  free(this);
}
