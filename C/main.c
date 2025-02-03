#include <stdio.h>
#include "ekf.h"

#include <stdlib.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 1024
#define DATA_SIZE 129908
#define SAMPLE_RATE 10

typedef struct {
    int sampled_size;
    double *t;
    double *Ah;
    double *I;
    double *SoC_CCM;
    double *T;
    double *V;
} SampledData;

void read_csv(const char *filename, SampledData *data) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    data->sampled_size = (DATA_SIZE + SAMPLE_RATE - 1) / SAMPLE_RATE;

    data->t = (double *)malloc(data->sampled_size * sizeof(double));
    data->Ah = (double *)malloc(data->sampled_size * sizeof(double));
    data->I = (double *)malloc(data->sampled_size * sizeof(double));
    data->SoC_CCM = (double *)malloc(data->sampled_size * sizeof(double));
    data->T = (double *)malloc(data->sampled_size * sizeof(double));
    data->V = (double *)malloc(data->sampled_size * sizeof(double));

    char line[MAX_LINE_LENGTH];
    fgets(line, MAX_LINE_LENGTH, file);

    int sampled_index = 0;
    for (int i = 0; i < DATA_SIZE; i++) {
        double t_val, Ah_val, I_val, SoC_CCM_val, T_val, V_val;
        fscanf(file, "%lf,%lf,%lf,%lf,%lf,%lf",
            &t_val, &Ah_val, &I_val, &SoC_CCM_val, &T_val, &V_val);
        
        if (i % SAMPLE_RATE == 0) {
            data->t[sampled_index] = t_val;
            data->Ah[sampled_index] = Ah_val;
            data->I[sampled_index] = I_val;
            data->SoC_CCM[sampled_index] = SoC_CCM_val;
            data->T[sampled_index] = T_val;
            data->V[sampled_index] = V_val;
            sampled_index++;
        }
    }

    fclose(file);
}

void plot_dual_variable(SampledData *data, const char *title, 
                        const char *ylabel, const char *label1, double *var1, 
                        const char *label2, double *var2) {
    FILE *gnuplot = popen("gnuplot -persistent", "w");
    if (gnuplot == NULL) {
        printf("Erro ao abrir o GNUplot.\n");
        exit(1);
    }

    fprintf(gnuplot, "set title '%s'\n", title);
    fprintf(gnuplot, "set xlabel 't'\n");
    fprintf(gnuplot, "set ylabel '%s'\n", ylabel);
    fprintf(gnuplot, "plot '-' with lines lw 2 title '%s', '-' with lines lw 2 title '%s'\n", label1, label2);

    for (int i = 0; i < data->sampled_size; i++) {
        fprintf(gnuplot, "%lf %lf\n", data->t[i], var1[i]);
    }
    fprintf(gnuplot, "e\n");

    for (int i = 0; i < data->sampled_size; i++) {
        fprintf(gnuplot, "%lf %lf\n", data->t[i], var2[i]);
    }
    fprintf(gnuplot, "e\n");

    pclose(gnuplot);
}

int main() {
    SampledData data;
    read_csv("dados.csv", &data);
    
    double Vt_Estimated = 0.0, Vt_Error = 0;
    double KG[2][1];
    double X[2][1];


    double P[2][2] = {{0.950000073892419, 0}, {0, 9.999996034254936}};
    double Q[2][2] = {{9.999944378671664e-08, 0}, {0, 9.999998929097178}}; 
    double R = 7.999987139962964;


    X[0][0]=1;
    X[1][0]=0;

    double prevI=-1;

    double *X0_history = (double *)malloc(data.sampled_size * sizeof(double));
    double *X1_history = (double *)malloc(data.sampled_size * sizeof(double));
    double *Vt_Estimated_history = (double *)malloc(data.sampled_size * sizeof(double));
    double *Vt_Error_history = (double *)malloc(data.sampled_size * sizeof(double));

    for (int i = 0; i < data.sampled_size; i++) {
        EKF_SOC_OPT(data.I[i], data.V[i], data.T[i], data.Ah[i], X, &Vt_Estimated, &Vt_Error, P, Q, R, KG,prevI, data.t[i]);
        X0_history[i] = X[0][0];
        X1_history[i] = X[1][0];
        Vt_Estimated_history[i] = Vt_Estimated;
        Vt_Error_history[i] = Vt_Error;
        prevI=data.I[i];
    }

    plot_dual_variable(&data, "SoC_CCM e X[0] vs t", "Valor", "SoC_CCM", data.SoC_CCM, "X[0]", X0_history);
    sleep(2);
    plot_dual_variable(&data, "Vt_Estimated e V vs t", "Tensão (V)", "Vt_Estimated", Vt_Estimated_history, "V", data.V);

    // Liberar memória
    free(data.t);
    free(data.Ah);
    free(data.I);
    free(data.SoC_CCM);
    free(data.T);
    free(data.V);
    free(X0_history);
    free(X1_history);
    free(Vt_Estimated_history);
    free(Vt_Error_history);
}

