#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include "resource.h"
#include "lab.h"
#include "infected.h"
#include "board.h"

int main() {
  printf("Começou!\n");
  lab *l_1 = create_lab("insumo secreto", "vírus", 0);
  lab *l_2 = create_lab("vírus", "injeção", 1);
  lab *l_3 = create_lab("injeção", "insumo secreto", 2);

  infected *in_1 = create_infected("vírus");
  infected *in_2 = create_infected("injeção");
  infected *in_3 = create_infected("insumo secreto");

  add_necessary_res(in_1, "injeção");
  add_necessary_res(in_1, "insumo secreto");

  add_necessary_res(in_2, "vírus");
  add_necessary_res(in_2, "insumo secreto");

  add_necessary_res(in_3, "injeção");
  add_necessary_res(in_3, "vírus");

  sem_t S[6];
  for (int i = 0; i < 6; i++) sem_init(&S[i], 0, 0);

  lab **labs = malloc(sizeof(lab *) * 3);
  infected **infs = malloc(sizeof(infected *) * 3);
  pthread_t thread_id[6];

  labs[0] = l_1;
  labs[1] = l_2;
  labs[2] = l_3;


  infs[0] = in_1;
  infs[1] = in_2;
  infs[2] = in_3;

  board *b = create_board(S, labs, infs, 10);
  printf("%d\n", get_laboratory_id(get_lab_with_board(b, 0)));
  printf("%d\n", get_laboratory_id(get_lab_with_board(b, 1)));
  printf("%d\n", get_laboratory_id(get_lab_with_board(b, 2)));
  // infectado
  params *p_1 = create_params(b, 0);
  params *p_2 = create_params(b, 1);
  params *p_3 = create_params(b, 2);

  // lab 
  params *lp_1 = create_params(b, 3);
  params *lp_2 = create_params(b, 3);
  params *lp_3 = create_params(b, 3);

  // int lab_count = 0;
  // printf("Entrou");

  // while(lab_count < 3) {
  //   printf("Entrou na inicialização dos labs");
  //   assign_resources(get_board(p_1), get_lab_with_board(get_board(p_1), lab_count));
  //   lab_count++;
  // }

  printf("Começou [2]\n");

  pthread_create(&thread_id[0], NULL, run_inf, p_1);
  pthread_create(&thread_id[1], NULL, run_inf, p_2);
  pthread_create(&thread_id[2], NULL, run_inf, p_3);
  pthread_create(&thread_id[3], NULL, run_lab, lp_1);
  pthread_create(&thread_id[4], NULL, run_lab, lp_2);
  pthread_create(&thread_id[5], NULL, run_lab, lp_3);

  printf("Começou [3]\n");
  for (int i = 0; i < 6; i++) pthread_join(thread_id[i], NULL);
  return 0;
}