#include "discrete_integrator.h"

void integrator_reset(discrete_integrator_t *integrator) {
    integrator->integral = 0.0;
    integrator->previous_value = 0.0;
    integrator->previous_time = 0;
}

double integrate_trapezoidal(discrete_integrator_t *integrator, double current_value, uint64_t current_time_us) {
    // Calcula o tempo decorrido desde a última amostra em segundos
    double delta_t = (current_time_us - integrator->previous_time) / 1e6;  // Converter para segundos

    // Calcula a integral pelo método do trapézio considerando o tempo
    double integral_value = 0.5 * (current_value + integrator->previous_value) * delta_t;
    integrator->integral += integral_value;

    // Atualiza os valores anteriores para a próxima iteração
    integrator->previous_value = current_value;
    integrator->previous_time = current_time_us;

    return integrator->integral;
}
