#ifndef __board_h__
#define __board_h__
#include <semaphore.h>
#include "cell.h"
#include "lab.h"

typedef struct board board;

board *create_board(sem_t sem[6], lab *labs[3]);

void assign_resources(board *this, lab *p_lab)

#endif