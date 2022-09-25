#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>

typedef struct resource
{
  int CLID; // Create lab id
  char *name;
} resource;

typedef struct thread_infected
{
  char *infinite_res;
  char *necessary_res[2];
  int count_my_res;
  int cycle_count;
} thread_infected;

typedef struct thread_lab
{
  int LID; // Lab Id
  char *res_1;
  char *res_2;
} thread_lab;

typedef struct cell
{
  int id;
  resource *res;
  sem_t *cell_sem;
} cell;

typedef struct board
{
  cell **board_cell;
  int lab_control[3];
} board;

typedef struct param_package
{
  thread_infected *t_inf;
  thread_lab *t_lab;
  board *board;
} param_package;

resource *make_first_resource(thread_lab *this)
{
  resource *new_res = (resource *)malloc(sizeof(resource));
  new_res->name = this->res_1;
  new_res->CLID = this->LID;

  return new_res;
}

resource *make_second_resource(thread_lab *this)
{
  resource *new_res = (resource *)malloc(sizeof(resource));
  new_res->name = this->res_2;
  new_res->CLID = this->LID;

  return new_res;
}

board *make_board(sem_t S[6])
{
  board *new_board = (board *)malloc(sizeof(board));
  new_board->board_cell = malloc(sizeof(cell *) * 6);

  for (int i = 0; i < 6; i++)
  {
    cell *current_cell = (cell *)malloc(sizeof(cell));
    current_cell->res = NULL;
    current_cell->cell_sem = &S[i];
    current_cell->id = i;

    new_board->board_cell[i] = current_cell;
  }

  for (int i = 0; i < 3; i++)
  {
    new_board->lab_control[i] = 0;
  }

  return new_board;
}

thread_infected *make_infected(char *infinite_res, char *necessary_res[2])
{
  thread_infected *new_inf = (thread_infected *)malloc(sizeof(thread_infected));
  new_inf->count_my_res = 0;
  new_inf->cycle_count = 0;
  new_inf->infinite_res = infinite_res;

  for (int i = 0; i < 2; i++)
  {
    new_inf->necessary_res[i] = necessary_res[i];
  }

  return new_inf;
}

thread_lab *make_lab(char *r1, char *r2, int LID)
{
  thread_lab *new_lab = (thread_lab *)malloc(sizeof(thread_infected));
  new_lab->LID = LID;
  new_lab->res_1 = r1;
  new_lab->res_2 = r2;
  return new_lab;
}

param_package *make_infected_param(board *b, thread_infected *inf)
{
  param_package *new_param = (param_package *)malloc(sizeof(param_package));

  new_param->board = b;
  new_param->t_inf = inf;
  new_param->t_lab = NULL;
  return new_param;
}

param_package *make_lab_param(board *b, thread_lab *lab)
{
  param_package *new_param = (param_package *)malloc(sizeof(param_package));

  new_param->board = b;
  new_param->t_lab = lab;
  new_param->t_inf = NULL;
  return new_param;
}

void *give_resource_to_infected(void *inf);

int main()
{
  // make structuries
  sem_t S[6];
  for (int i = 0; i < 6; i++)
    sem_init(&S[i], 0, 1);

  board *b = make_board(S);

  char *n1[2];
  n1[0] = "insumo secreto";
  n1[1] = "seringa";
  thread_infected *inf_1 = make_infected("vírus", n1);

  char *n2[2];
  n2[0] = "vírus";
  n2[1] = "seringa";
  thread_infected *inf_2 = make_infected("insumo secreto", n2);

  char *n3[2];
  n3[0] = "insumo secreto";
  n3[1] = "vírus";
  thread_infected *inf_3 = make_infected("seringa", n3);

  thread_lab *lab_1 = make_lab("vírus", "injeção", 0);
  thread_lab *lab_2 = make_lab("vírus", "insumo secreto", 1);
  thread_lab *lab_3 = make_lab("insumo secreto", "injeção", 2);

  param_package **tt = malloc(sizeof(param_package *) * 6);
  param_package *p_inf_1 = make_infected_param(b, inf_1);
  tt[0] = p_inf_1;
  param_package *p_inf_2 = make_infected_param(b, inf_2);
  tt[1] = p_inf_2;
  param_package *p_inf_3 = make_infected_param(b, inf_3);
  tt[2] = p_inf_3;
  param_package *p_lab_1 = make_lab_param(b, lab_1);
  tt[3] = p_lab_1;
  param_package *p_lab_2 = make_lab_param(b, lab_2);
  tt[4] = p_lab_2;
  param_package *p_lab_3 = make_lab_param(b, lab_3);
  tt[5] = p_lab_3;
  pthread_t thread_id[6];

  pthread_create(&(thread_id[0]), NULL, give_resource_to_infected, p_inf_1);
  pthread_create(&(thread_id[1]), NULL, give_resource_to_infected, p_inf_2);
  pthread_create(&(thread_id[2]), NULL, give_resource_to_infected, p_inf_3);

  // for (int i = 0; i < 6; i++) {
  //   pthread_create(&thread_id[i], NULL, give_resource_to_infected, tt[i]);  
  // }
  pthread_create(&(thread_id[3]), NULL, give_resource_to_infected, p_lab_1);
  pthread_create(&(thread_id[4]), NULL, give_resource_to_infected, p_lab_2);
  pthread_create(&(thread_id[5]), NULL, give_resource_to_infected, p_lab_3);

  for (int i = 0; i < 6; i++) {
    pthread_join(thread_id[i], NULL);
  }

  return 0;
}

void *give_resource_to_infected(void *param)
{
  param_package *package = (param_package *)param;
  
  if (package->t_inf != NULL) {
    printf("Teste\n");
    printf("\n%s\n", package->t_inf->infinite_res);
  } else {
    printf("%d\n", package->t_lab->LID);
  }

  if (package->t_inf != NULL)
  {
    for (int i = 0; i < 6; i++)
    {
      for (int j = 0; j < 2; j++)
      {
        if (package->board->board_cell[i]->res != NULL && package->t_inf->necessary_res[j] == package->board->board_cell[i]->res->name)
        {
          sem_wait(package->board->board_cell[i]->cell_sem);
          package->t_inf->count_my_res += 1;
          package->board->lab_control[package->board->board_cell[i]->res->CLID] -= 1;
          resource *aux = package->board->board_cell[i]->res;
          package->board->board_cell[i]->res = NULL;
          free(aux);
          if (package->t_inf->count_my_res == 2)
          {
            package->t_inf->cycle_count += 1;
            package->t_inf->count_my_res = 0;
          }
          sem_post(package->board->board_cell[i]->cell_sem);
        }
      }
    }
  }

  if (package->t_lab != NULL)
  {
    for (int i = 0; i < 3; i++)
    {
      if (package->board->lab_control[i] == 0)
      {
        resource *res_1 = make_first_resource(package->t_lab);
        resource *res_2 = make_second_resource(package->t_lab);
        int give_f_res = 0;
        int give_s_res = 0;
        for (int j = 0; j < 6; j++)
        {
          if (package->board->board_cell[j]->res == NULL)
          {
            if (package->board->lab_control[i] < 2)
            {
              sem_wait(package->board->board_cell[j]->cell_sem);
              if (give_f_res == 0)
              {
                package->board->board_cell[j]->res = res_1;
                give_f_res = 1;
              }

              if (give_s_res == 0)
              {
                package->board->board_cell[j]->res = res_2;
                give_s_res = 1;
              }

              package->board->lab_control[i] += 1;
              sem_post(package->board->board_cell[j]->cell_sem);
            }
          }
        }
      }
    }
  }
}