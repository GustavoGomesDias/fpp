#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "cell.h"

struct cell {
  resource *res;
  sem_t sem;
};

cell *create_board_cell(sem_t sem) {
  cell *new_cell = (cell *) malloc(sizeof(cell));

  new_cell->res = NULL;
  new_cell->sem = sem;

  return new_cell;
}

void run_sem_wait(cell *this) {
  sem_wait(&this->sem);
}

void run_sem_post(cell *this) {
  sem_post(&this->sem);
}

void set_res(cell *this, resource *res) {
  this->res = res;
}

void remove_res(cell *this) {
  this->res == NULL;
}

resource *get_cell_res(cell *this) {
  return this->res;
}