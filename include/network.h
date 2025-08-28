#pragma once

#include <stdbool.h>

// ======================
// Type Definitions
// ======================

typedef struct neuron_t {
  int n_inputs;
  double *weights;
  double bias;
  double output;
} neuron_t;

typedef struct network_t {
  int n_inputs;
  int n_hidden;
  int n_outputs;
  neuron_t *hidden;
  neuron_t *output;
} network_t;

// ======================
// Neuron Functions
// ======================

neuron_t *new_neuron(int n_inputs);
bool update_neuron(neuron_t *n, const double *inputs);
bool destroy_neuron(neuron_t *n);

// ======================
// Network Functions
// ======================

network_t *new_network(int n_inputs, int n_hidden, int n_outputs);
bool update_network(network_t *net, const double *inputs, double *outputs);
bool destroy_network(network_t *net);
