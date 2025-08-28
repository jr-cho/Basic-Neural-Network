#include "../include/network.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// ========== Helper: Random Double ==========
static inline double rand_double(void) {
  return ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0; // [-1,1]
}

// ========== Activation Function ==========
static inline double sigmoid(double x) { return 1.0 / (1.0 + exp(-x)); }

// ========== Neuron Implementation ==========
neuron_t *new_neuron(int n_inputs) {
  if (n_inputs <= 0)
    return NULL;

  neuron_t *n = malloc(sizeof(neuron_t));
  if (!n)
    return NULL;

  n->n_inputs = n_inputs;
  n->weights = malloc(sizeof(double) * n_inputs);
  if (!n->weights) {
    free(n);
    return NULL;
  }

  for (int i = 0; i < n_inputs; i++)
    n->weights[i] = rand_double();

  n->bias = rand_double();
  n->output = 0.0;

  return n;
}

bool update_neuron(neuron_t *n, const double *inputs) {
  if (!n || !inputs)
    return false;

  double sum = n->bias;
  for (int i = 0; i < n->n_inputs; i++) {
    sum += n->weights[i] * inputs[i];
  }

  n->output = sigmoid(sum);
  return true;
}

bool destroy_neuron(neuron_t *n) {
  if (!n)
    return false;
  free(n->weights);
  free(n);
  return true;
}

// ========== Network Implementation ==========
network_t *new_network(int n_inputs, int n_hidden, int n_outputs) {
  if (n_inputs <= 0 || n_hidden <= 0 || n_outputs <= 0)
    return NULL;

  network_t *net = malloc(sizeof(network_t));
  if (!net)
    return NULL;

  net->n_inputs = n_inputs;
  net->n_hidden = n_hidden;
  net->n_outputs = n_outputs;

  net->hidden = malloc(sizeof(neuron_t) * n_hidden);
  if (!net->hidden) {
    free(net);
    return NULL;
  }
  for (int i = 0; i < n_hidden; i++) {
    neuron_t *n = new_neuron(n_inputs);
    if (!n) {
      destroy_network(net);
      return NULL;
    }
    net->hidden[i] = *n;
    free(n);
  }

  net->output = malloc(sizeof(neuron_t) * n_outputs);
  if (!net->output) {
    destroy_network(net);
    return NULL;
  }
  for (int i = 0; i < n_outputs; i++) {
    neuron_t *n = new_neuron(n_hidden);
    if (!n) {
      destroy_network(net);
      return NULL;
    }
    net->output[i] = *n;
    free(n);
  }

  return net;
}

bool update_network(network_t *net, const double *inputs, double *outputs) {
  if (!net || !inputs || !outputs)
    return false;

  double *hidden_outputs = malloc(sizeof(double) * net->n_hidden);
  if (!hidden_outputs)
    return false;

  for (int i = 0; i < net->n_hidden; i++) {
    if (!update_neuron(&net->hidden[i], inputs)) {
      free(hidden_outputs);
      return false;
    }
    hidden_outputs[i] = net->hidden[i].output;
  }

  for (int i = 0; i < net->n_outputs; i++) {
    if (!update_neuron(&net->output[i], hidden_outputs)) {
      free(hidden_outputs);
      return false;
    }
    outputs[i] = net->output[i].output;
  }

  free(hidden_outputs);
  return true;
}

bool destroy_network(network_t *net) {
  if (!net)
    return false;

  if (net->hidden) {
    for (int i = 0; i < net->n_hidden; i++)
      free(net->hidden[i].weights);
    free(net->hidden);
  }

  if (net->output) {
    for (int i = 0; i < net->n_outputs; i++)
      free(net->output[i].weights);
    free(net->output);
  }

  free(net);
  return true;
}
