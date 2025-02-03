
#include "mex.h"
#include "ekf.h"


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    if (nrhs != 6) {
        mexErrMsgIdAndTxt("MATLAB:apriori_mex:invalidNumInputs", "Seis entradas são esperadas: A, B, i, X, Q, P.");
    }
    if (nlhs != 2) {
        mexErrMsgIdAndTxt("MATLAB:apriori_mex:maxlhs", "Duas saídas são esperadas: X atualizado e P atualizado.");
    }

    // Obtendo entradas
    double *A = mxGetPr(prhs[0]);
    double *B = mxGetPr(prhs[1]);
    double i = mxGetScalar(prhs[2]);
    double *X = mxGetPr(prhs[3]);
    double *Q = mxGetPr(prhs[4]);
    double *P = mxGetPr(prhs[5]);

    // Criando saídas (cópia de X e P para modificar)
    plhs[0] = mxDuplicateArray(prhs[3]);
    plhs[1] = mxDuplicateArray(prhs[5]);

    double *X_out = mxGetPr(plhs[0]);
    double *P_out = mxGetPr(plhs[1]);

    // Chamando a função Apriori
    Apriori((double (*)[2])A, (double (*)[2])B, i, (double (*)[2])X_out, (double (*)[2])Q, (double (*)[2])P_out);
}