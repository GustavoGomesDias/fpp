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
  char *my_res[2];
  int count_my_res;
  int cycle_count;
  sem_t *run_controll;
} thread_infected;

typedef struct thread_lab
{
  int LID; // Lab Id
  char *res_1;
  char *res_2;
  int cycle_count;
} thread_lab;

typedef struct cell
{
  int id;
  resource *res;
  sem_t *cell_sem;
} cell;

typedef struct board
{
  thread_infected **infecteds;
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
  new_board->infecteds = malloc(sizeof(thread_infected *) * 3);

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

thread_infected *make_infected(char *infinite_res, char *necessary_res[2], sem_t run_control)
{
  thread_infected *new_inf = (thread_infected *)malloc(sizeof(thread_infected));
  new_inf->count_my_res = 0;
  new_inf->cycle_count = 0;
  new_inf->infinite_res = infinite_res;
  new_inf->run_controll = &run_control;

  for (int i = 0; i < 2; i++)
  {
    new_inf->necessary_res[i] = necessary_res[i];
    new_inf->my_res[i] = "";
  }

  return new_inf;
}

int check_if_res_exists_in_my_res(thread_infected *this, char *res_name)
{
  for (int i = 0; i < 2; i++)
  {
    if (this->my_res[i] == res_name)
      return 1;
  }

  return 0;
}

void add_in_infected_res(thread_infected *this, char *name)
{
  for (int i = 0; i < 2; i++)
  {
    if (strcmp(this->my_res[i], "") == 0)
    {
      this->my_res[i] = name;
      this->count_my_res += 1;
    }
  }

  return;
}

void clear_infected_res(thread_infected *this)
{
  for (int i = 0; i < 2; i++)
  {
    this->my_res[i] = "";
  }
  this->count_my_res = 0;
}

thread_lab *make_lab(char *r1, char *r2, int LID)
{
  thread_lab *new_lab = (thread_lab *)malloc(sizeof(thread_infected));
  new_lab->LID = LID;
  new_lab->res_1 = r1;
  new_lab->res_2 = r2;
  new_lab->cycle_count = 0;
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

void *infected_thread_func(void *param)
{
  printf("Aqui foi [inf]\n");
  param_package *package = (param_package *)param;
  sem_wait(package->t_inf->run_controll);
  printf("Liberei o acesso [inf]\n");

  for (int i = 0; i < 6; i++)
  {
    if (package->board->board_cell[i]->res != NULL)
    {
      printf("Entrei na célula\n");
      sem_wait(package->board->board_cell[i]->cell_sem);
      printf("i: %d\n", i);

      for (int j = 0; j < 2; j++)
      {
        printf("Célula me liberou\n");
        if (package->t_inf->necessary_res[j] == package->board->board_cell[i]->res->name)
        {
          printf("Passei da verificação\n");
          if (check_if_res_exists_in_my_res(package->t_inf, package->board->board_cell[i]->res->name) == 0)
          {
            add_in_infected_res(package->t_inf, package->board->board_cell[i]->res->name);
            printf("Geted in lab %d\n", package->board->board_cell[i]->res->CLID);
            package->board->lab_control[package->board->board_cell[i]->res->CLID] -= 1;

            if (package->t_inf->count_my_res == 2)
            {
              package->t_inf->cycle_count += 1;
              clear_infected_res(package->t_inf);
            }
            printf("Necessary res: %s\n", package->t_inf->necessary_res[j]);
          }
        }
      }

      resource *aux = package->board->board_cell[i]->res;
      package->board->board_cell[i]->res = NULL;
      free(aux);
      printf("Saindo...\n");
      sem_post(package->board->board_cell[i]->cell_sem);
    }
  }
  printf("Terminei\n");

  return NULL;
}

void *lab_thread_func(void *param)
{
  printf("Aqui foi [lab]\n");
  param_package *package = (param_package *)param;
  while (package->t_lab->cycle_count < 10)
  {
    if (package->board->lab_control[package->t_lab->LID] == 0)
    {
      resource *res_1 = make_first_resource(package->t_lab);
      resource *res_2 = make_second_resource(package->t_lab);
      int give_f_res = 0;
      int give_s_res = 0;
      for (int j = 0; j < 6; j++)
      {
        if (package->board->board_cell[j]->res == NULL)
        {
          sem_wait(package->board->board_cell[j]->cell_sem);
          if (package->board->lab_control[package->t_lab->LID] < 2)
          {
            if (give_f_res == 0 && package->board->board_cell[j]->res == NULL)
            {
              package->board->board_cell[j]->res = res_1;
              printf("Added %d: %s\n", j, package->board->board_cell[j]->res->name);
              give_f_res = 1;
            }

            if (give_s_res == 0 && package->board->board_cell[j]->res == NULL)
            {
              package->board->board_cell[j]->res = res_2;
              printf("Added %d: %s\n", j, package->board->board_cell[j]->res->name);
              give_s_res = 1;
            }
            package->t_lab->cycle_count += 1;

            package->board->lab_control[package->t_lab->LID] += 1;
          }
          sem_post(package->board->board_cell[j]->cell_sem);
        }
      }
    }
  }

  board *b = package->board;
  char *aux = "";
  int count = 0;
  for (int j = 0; j < 6; j++)
  {
    if (count == 2)
    {

      sem_post(b->infecteds[0]->run_controll);
      return NULL;
    }

    for (int k; k < 2; k++)
    {
      if (strcmp(b->infecteds[0]->necessary_res[k], b->board_cell[j]->res->name) && !strcmp(aux, b->board_cell[j]->res->name))
      {
        count++;
        aux = b->board_cell[6]->res->name;
      }
    }
  }

  // for (int i = 0; i < 3; i++)
  // {
  //   char *aux = "";
  //   int count = 0;
  //   for (int j = 0; j < 6; j++)
  //   {
  //     if (count == 2)
  //     {

  //       sem_post(b->infecteds[i]->run_controll);
  //       return NULL;
  //     }

  //     for (int k; k < 2; k++)
  //     {
  //       if (strcmp(b->infecteds[i]->necessary_res[k], b->board_cell[6]->res->name) && !strcmp(aux, b->board_cell[6]->res->name))
  //       {
  //         count++;
  //         aux = b->board_cell[6]->res->name;
  //       }
  //     }
  //   }
  // }
}

int main()
{
  // make structuries
  sem_t S[6];
  sem_t inf_ctr[3];
  for (int i = 0; i < 6; i++)
    sem_init(&S[i], 0, 1);
  for (int i = 0; i < 3; i++)
    sem_init(&inf_ctr[i], 0, 1);

  board *b = make_board(S);

  char *n1[2];
  n1[0] = "insumo secreto";
  n1[1] = "seringa";
  thread_infected *inf_1 = make_infected("virus", n1, inf_ctr[0]);

  char *n2[2];
  n2[0] = "virus";
  n2[1] = "seringa";
  thread_infected *inf_2 = make_infected("insumo secreto", n2, inf_ctr[1]);

  char *n3[2];
  n3[0] = "insumo secreto";
  n3[1] = "virus";
  thread_infected *inf_3 = make_infected("seringa", n3, inf_ctr[2]);
  b->infecteds[0] = inf_1;
  b->infecteds[1] = inf_2;
  b->infecteds[2] = inf_3;

  thread_lab *lab_1 = make_lab("insumo secreto", "seringa", 0);
  thread_lab *lab_2 = make_lab("virus", "seringa", 1);
  thread_lab *lab_3 = make_lab("insumo secreto", "virus", 2);

  param_package *p_inf_1 = make_infected_param(b, inf_1);
  // param_package *p_inf_2 = make_infected_param(b, inf_2);
  // param_package *p_inf_3 = make_infected_param(b, inf_3);
  param_package *p_lab_1 = make_lab_param(b, lab_1);
  // param_package *p_lab_2 = make_lab_param(b, lab_2);
  // param_package *p_lab_3 = make_lab_param(b, lab_3);
  pthread_t thread_id[2];

  pthread_create(&(thread_id[0]), NULL, lab_thread_func, p_lab_1);
  pthread_create(&(thread_id[1]), NULL, infected_thread_func, p_inf_1);
  // pthread_create(&(thread_id[1]), NULL, infected_thread_func, p_inf_2);
  // pthread_create(&(thread_id[2]), NULL, infected_thread_func, p_inf_3);

  // pthread_create(&(thread_id[3]), NULL, lab_thread_func, p_lab_1);
  // pthread_create(&(thread_id[4]), NULL, lab_thread_func, p_lab_2);

  for (int i = 0; i < 2; i++)
  {
    pthread_join(thread_id[i], NULL);
  }

  return 0;
}
