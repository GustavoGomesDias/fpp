#ifndef __board_h__
#define __board_h__
#include <semaphore.h>
#include "cell.h"
#include "lab.h"
#include "infected.h"

typedef struct board board;
typedef struct params params;

board *create_board(sem_t sem[6], lab **labs, infected **infs, int max_cycle);
params *create_params(board *board, int inf_pos);

int get_max_cycle(board *this);

void assign_resources(board *this, lab *p_lab);

void give_res_to_infected(board *this, infected *inf, cell *c);

void *run_inf(void *p);

void *run_lab(void *p);

board *get_board(params *this);

lab *get_lab_with_board(board *this, int lab_pos);
#endif