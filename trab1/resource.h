#ifndef __resource_h__
#define __resource_h__

typedef struct resource resource;

resource *create_resource(char res_name[], int create_lab);

int get_lab_id(resource *this);

resource *get_next_res(resource *res);

char *get_name(resource *this);

void set_next_res(resource *this, resource *next_res);

void delete_res(resource *this);

#endif