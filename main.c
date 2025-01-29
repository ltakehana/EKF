#include <stdio.h>
#include "battery_model.h"


int main() {
    double i = 1.0, V1 = 3.7, T = 25.0, SOC = 0.51;
    int Nc = 10;
    double A[2][2], b[1][2], c[2][1], Vt;

    battery_model(i, V1, T, SOC, Nc, A, b, c, &Vt);
    
    printf("Matriz A: \n");
    printf("[%lf, %lf]\n", A[0][0], A[0][1]);
    printf("[%lf, %lf]\n", A[1][0], A[1][1]);

    printf("Matriz b: \n");
    printf("[%lf, %lf]\n", b[0][0], b[0][1]);
    
    printf("Matriz c: \n");
    printf("[%lf]\n", c[0][0]);
    printf("[%lf]\n", c[1][0]);
    
    printf("Vt: %lf\n", Vt);

    return 0;
}

