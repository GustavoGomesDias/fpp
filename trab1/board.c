#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

struct board {
  int control_lab_res[3];
  lab **labs;
  infected **infs;
  cell **board_cells;
  int max_cycle;
};

struct params {
  struct board *b;
  int inf_pos;
};

params *create_params(board *b, int inf_pos) {
  params *new_params = (params *) malloc(sizeof(params));
  new_params->b = NULL;
  new_params->b = b;
  new_params->inf_pos = inf_pos;

  return new_params;
}

int get_max_cycle(board *this) {
  return this->max_cycle;
}

board *create_board(sem_t sem[6], lab **labs, infected **infs, int max_cycle) {
  board *new_board = (board *) malloc(sizeof(board));

  new_board->board_cells = malloc(sizeof(cell *) * 6);
  new_board->labs = malloc(sizeof(lab *) * 3);
  new_board->infs = malloc(sizeof(infected *) * 3);

  for (int i = 0; i < 3; i++) {
    new_board->labs[i] = labs[i];
    new_board->infs[i] = infs[i];
  }

  new_board->max_cycle = max_cycle;


  for (int i = 0; i < 6; i++) {
    cell *current_cell = create_board_cell(sem[i]);
    new_board->board_cells[i] = current_cell;
  }

  for (int j = 0; j < 3; j++) {
    new_board->control_lab_res[j] = 0;
  }

  return new_board;
}

void assign_resources(board *this, lab *p_lab) {
  int res_list_pos = 0;
  resource **res_list = make_all_resources(p_lab);
  for (int i = 0; i < 6; i++) {
    if (i < 6 && get_cell_res(this->board_cells[i]) == NULL) {
      run_sem_wait(this->board_cells[i]);
      
      set_res(this->board_cells[i], res_list[res_list_pos]);
      this->control_lab_res[get_lab_id(res_list[res_list_pos])] += 1;
      res_list_pos++;

      run_sem_post(this->board_cells[i]);
      this->control_lab_res[get_laboratory_id(p_lab)] += 1;

      if (res_list_pos == 2) {
        set_lab_cycle_count(p_lab);
      }
    }

    if (res_list_pos == 2) {
      return;
    }
  }

  return;
}


// TODO: Problema no wait

void give_res_to_infected(board *this, infected *inf, cell *c) {

  for (int i = 0; i < 2; i++) {
    printf("Run infected [%d]\n", i);
    printf("%s\n", get_necessary_res(inf, i));
    printf("infinite res: %s", get_inf_res(inf));
    run_sem_wait(c);
    if (get_necessary_res(inf, i) == get_name(get_cell_res(c))) {
      add_my_res(inf, get_cell_res(c));
      make_solution(inf);
      remove_res(c);
      this->control_lab_res[get_lab_id(get_cell_res(c))] -= 1;
      set_infected_cycle_count(inf);

      printf("Cycle count: %d", get_infected_cycle_count(inf));
      printf("Inf resourse: %s", get_inf_res(inf));
    }
    run_sem_post(c);
  }
}


void *run_inf(void *p) {
  params *param = (params *) p;

  // printf("Saiu\n");
  while (get_infected_cycle_count(param->b->infs[param->inf_pos]) < param->b->max_cycle) {
    for (int i = 0; i < 6; i++) {
      give_res_to_infected(param->b, param->b->infs[param->inf_pos], param->b->board_cells[i]);
    }
  }

  return NULL;
}

void *run_lab(void *p) {
  params *param = (params *) p;
  int lab_count = 0;

  while(param->b->control_lab_res[lab_count] == 0) {
    assign_resources(param->b, param->b->labs[lab_count]);
    lab_count++;
  }

  return NULL;
}

board *get_board(params *this) {
  return this->b;
}

lab *get_lab_with_board(board *this, int lab_pos) {
  return this->labs[lab_pos];
}
