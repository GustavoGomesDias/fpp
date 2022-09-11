#include <stdio.h>
#include <stdlib.h>
#include "infected.h"


struct infected {
  int num_production;
  resource *infinite_res;
  resource *necessary_res;
  resource *my_res;
};
