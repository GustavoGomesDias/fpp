#ifndef __cell_h__
#define __cell_h__
#include "resource.h"

typedef struct cell cell;

cell *create_board_cell(sem_t sem);

void run_sem_wait(cell *this);

void run_sem_post(cell *this);

void set_res(cell *this, resource *res);

void remove_res(cell *this);

resource *get_cell_res(cell *this);

#endif