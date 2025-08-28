#include "../include/network.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  srand((unsigned int)time(NULL));

  network_t *net = new_network(2, 3, 1);
  if (!net) {
    fprintf(stderr, "Failed to create network\n");
    return 1;
  }

  double inputs[2] = {1.0, 0.0};
  double outputs[1];

  if (!update_network(net, inputs, outputs)) {
    fprintf(stderr, "Failed to update network\n");
    destroy_network(net);
    return 1;
  }

  printf("Output: %f\n", outputs[0]);

  if (!destroy_network(net)) {
    fprintf(stderr, "Failed to free network\n");
    return 1;
  }

  return 0;
}
