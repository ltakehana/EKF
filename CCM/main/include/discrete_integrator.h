#ifndef DISCRETE_INTEGRATOR_H
#define DISCRETE_INTEGRATOR_H

#include <unistd.h>

typedef struct {
    double integral;         // Valor da integral acumulada
    double previous_value;   // Valor da amostra anterior
    uint64_t previous_time;  // Tempo da amostra anterior (em microssegundos)
} discrete_integrator_t;

void integrator_reset(discrete_integrator_t *integrator);
double integrate_trapezoidal(discrete_integrator_t *integrator, double current_value, uint64_t current_time_us);

#endif // DISCRETE_INTEGRATOR_H
