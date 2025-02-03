#include "mex.h"
#include "ekf.h"

// Implementação da interface MEX
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    if (nrhs != 5) {
        mexErrMsgIdAndTxt("MATLAB:aposteriori_mex:invalidNumInputs", "Cinco entradas são esperadas: X, P, C, R, Error_x.");
    }
    if (nlhs != 3) {
        mexErrMsgIdAndTxt("MATLAB:aposteriori_mex:maxlhs", "Três saídas são esperadas: X atualizado, P atualizado e KG.");
    }

    // Obtendo entradas do MATLAB
    double *X = mxGetPr(prhs[0]);
    double *P = mxGetPr(prhs[1]);
    double *C = mxGetPr(prhs[2]);
    double R = mxGetScalar(prhs[3]);
    double Error_x = mxGetScalar(prhs[4]);

    // Criando saídas (cópias de X e P para modificar)
    plhs[0] = mxDuplicateArray(prhs[0]);
    plhs[1] = mxDuplicateArray(prhs[1]);
    plhs[2] = mxCreateDoubleMatrix(2, 1, mxREAL); // KG (2x1)

    double *X_out = mxGetPr(plhs[0]);
    double *P_out = mxGetPr(plhs[1]);
    double *KG_out = mxGetPr(plhs[2]);

    // Chamando a função Aposteriori
    Aposteriori((double (*)[2])X_out, (double (*)[2])P_out, (double (*)[1])C, R, Error_x, (double (*)[2])KG_out);
}