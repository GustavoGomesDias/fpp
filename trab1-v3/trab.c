#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

typedef int resource;

typedef struct run_control
{
  int *labs;
  int *infecteds;
  int max_value;
} run_control;

typedef struct th_infected
{
  // Run control
  int IID;
  run_control *rc;

  // Resourcers
  resource f;
  resource f2;
  resource s;
  resource s2;

  // Thread control
  pthread_t thread_id;
  pthread_mutex_t *mutex;
  sem_t *sem_board;

  int *board;
} th_infected;

typedef struct th_lab
{
  int LID;
  run_control *rc;

  pthread_t thread_id;
  resource f;
  resource s;
  sem_t *sem_board;

  int *board;
} th_lab;

th_infected *make_infected(int id, run_control *rc, resource f, resource f2, resource s, resource s2, pthread_mutex_t *mutex, sem_t *sem_board, int board[6])
{
  th_infected *inf = (th_infected *) malloc(sizeof(th_infected));
  inf->IID = id;
  inf->f = f;
  inf->f2 = f2;
  inf->s = s;
  inf->s2 = s2;
  inf->rc = rc;
  inf->mutex = mutex;
  inf->sem_board = sem_board;
  inf->board = (int *)malloc(6 * sizeof(int));
  inf->board = board;

  return inf;
}

th_lab *make_lab(int LID, run_control *rc, resource f, resource s, sem_t *sem_board, int board[6])
{
  th_lab *lab = (th_lab *) malloc(sizeof(th_lab));

  lab->LID = LID;
  lab->rc = rc;
  lab->f = f;
  lab->s = s;
  lab->sem_board = sem_board;
  lab->board = (int *)malloc(6 * sizeof(int));
  lab->board = board;

  return lab;
}

int check_inf_has_finished(run_control *rc)
{
  if (rc->infecteds[0] >= rc->max_value && rc->infecteds[1] >= rc->max_value && rc->infecteds[2] >= rc->max_value)
  {
    return 1;
  }

  return 0;
}

void *th_func_infected(void *inf)
{
  th_infected *inft = (th_infected *)inf;

  while (check_inf_has_finished(inft->rc) == 0)
  {
    int res_count = 0;
    int assign_f = 0;
    int assign_s = 0;
    int board_v_1, board_v_2, board_v_3, board_v_4;
    sem_getvalue(&inft->sem_board[inft->f], &board_v_1);
    sem_getvalue(&inft->sem_board[inft->f2], &board_v_2);
    sem_getvalue(&inft->sem_board[inft->s], &board_v_3);
    sem_getvalue(&inft->sem_board[inft->s2], &board_v_4);

    if (res_count < 2 && assign_f == 0 && board_v_1 == 1)
    {
      sem_wait(&inft->sem_board[inft->f]);
      if (inft->board[inft->f] != 0)
      {
        assign_f++;
        res_count++;
        inft->board[inft->f] = 0;
      }
      sem_post(&inft->sem_board[inft->f]);
    }

    if (res_count < 2 && assign_f == 0 && board_v_2 == 1)
    {
      sem_wait(&inft->sem_board[inft->f2]);
      if (inft->board[inft->f2] != 0)
      {
        assign_f++;
        res_count++;
        inft->board[inft->f2] = 0;
      }
      sem_post(&inft->sem_board[inft->f2]);
    }

    if (res_count < 2 && assign_s == 0 && board_v_3 == 1)
    {
      sem_wait(&inft->sem_board[inft->s]);
      if (inft->board[inft->s] != 0)
      {
        assign_s++;
        res_count++;
        inft->board[inft->s] = 0;
      }
      sem_post(&inft->sem_board[inft->s]);
    }

    if (res_count < 2 && assign_s == 0 && board_v_4 == 1)
    {
      sem_wait(&inft->sem_board[inft->s2]);
      if (inft->board[inft->s2] != 0)
      {
        assign_s++;
        res_count++;
        inft->board[inft->s2] = 0;
      }
      sem_post(&inft->sem_board[inft->s2]);
    }

    if (res_count >= 2)
    {
      int count = inft->rc->infecteds[inft->IID] + 1;

      inft->rc->infecteds[inft->IID] = count;
    }
  }
  return NULL;
}

void *th_func_lab(void *lab)
{
  th_lab *tlab = (th_lab *) lab;

  while (check_inf_has_finished(tlab->rc) == 0)
  {
    int board_v_1, board_v_2;
    sem_getvalue(&tlab->sem_board[tlab->f], &board_v_1);
    sem_getvalue(&tlab->sem_board[tlab->s], &board_v_2);

    if (board_v_1 == 1 && board_v_2 == 1)
    {

      if (tlab->board[tlab->f] == 0 && tlab->board[tlab->s] == 0)
      {
        sem_wait(&tlab->sem_board[tlab->f]);
        tlab->board[tlab->f] = tlab->f;
        sem_post(&tlab->sem_board[tlab->f]);

        sem_wait(&tlab->sem_board[tlab->s]);
        tlab->board[tlab->s] = tlab->s;
        sem_post(&tlab->sem_board[tlab->s]);

        int count = tlab->rc->labs[tlab->LID] + 1;

        tlab->rc->labs[tlab->LID] = count;
      }
    }
  }
  return NULL;
}

int main(int argc,char* argv[])
{
  run_control *rc = (run_control *) malloc(sizeof(run_control));
  rc->infecteds = (int *) malloc(3 * sizeof(int));
  rc->labs = (int *) malloc(3 * sizeof(int));
  pthread_mutex_t mutex;

  for (int i = 0; i < 3; i++)
  {
    rc->infecteds[i] = 0;
    rc->labs[i] = 0;
  }

  rc->max_value = atoi(argv[1]);

  sem_t *sem_board = (sem_t *)malloc(sizeof(sem_t) * 6);

  for (int i = 0; i < 6; i++)
  {
    sem_init(&sem_board[i], 0, 1);
  }

  resource virus1 = 0;
  resource virus2 = 1;
  resource secret_res1 = 2;
  resource secret_res2 = 3;
  resource injection1 = 4;
  resource injection2 = 5;

  int board[6] = {0, 0, 0, 0, 0, 0};

  th_infected *if0 = make_infected(0, rc, virus1, virus2, secret_res1, secret_res2, &mutex, sem_board, board);
  th_infected *if1 = make_infected(1, rc, virus1, virus2, injection1, injection2, &mutex, sem_board, board);
  th_infected *if2 = make_infected(2, rc, secret_res1, secret_res2, injection1, injection2, &mutex, sem_board, board);

  th_lab *l0 = make_lab(0, rc, virus1, secret_res1, sem_board, board);
  th_lab *l1 = make_lab(1, rc, virus2, injection1, sem_board, board);
  th_lab *l2 = make_lab(2, rc, secret_res2, injection2, sem_board, board);

  pthread_create(&if0->thread_id, NULL, th_func_infected, if0);
  pthread_create(&if1->thread_id, NULL, th_func_infected, if1);
  pthread_create(&if2->thread_id, NULL, th_func_infected, if2);

  pthread_create(&l0->thread_id, NULL, th_func_lab, l0);
  pthread_create(&l1->thread_id, NULL, th_func_lab, l1);
  pthread_create(&l2->thread_id, NULL, th_func_lab, l2);

  pthread_join(l0->thread_id, NULL);
  pthread_join(l1->thread_id, NULL);
  pthread_join(l2->thread_id, NULL);

  pthread_join(if0->thread_id, NULL);
  pthread_join(if1->thread_id, NULL);
  pthread_join(if2->thread_id, NULL);

  printf("Inf 1: %d\n", rc->infecteds[0]);
  printf("Inf 2: %d\n", rc->infecteds[1]);
  printf("Inf 3: %d\n", rc->infecteds[2]);

  printf("Lab 1: %d\n", rc->labs[0]);
  printf("Lab 2: %d\n", rc->labs[1]);
  printf("Lab 3: %d\n", rc->labs[2]);

  return 0;
}
