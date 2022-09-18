#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

struct board {
  int control_lab_res[3];
  lab **labs;
  cell **board_cells;
};


board *create_board(sem_t sem[6], lab **labs) {
  board *new_board = (board *) malloc(sizeof(board));

  new_board->board_cells = malloc(sizeof(cell *) * 6);
  new_board->labs = labs;


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
      this->control_lab_res[get_lab_id(p_lab)] += 1;

      if (res_list_pos == 2) {
        set_lab_cycle_count(p_lab);
      }
    }

    if (res_list_pos == 2) {
      return;
    }
  }
}

// infected get resource
