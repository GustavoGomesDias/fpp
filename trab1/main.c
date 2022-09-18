#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include "resource.h"
#include "lab.h"
#include "infected.h"

int main() {
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

  // labs make resources
  return 0;
}