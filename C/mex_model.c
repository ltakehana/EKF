#include "mex.h"
#include "battery_model.h" 

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    if (nrhs != 5) {
        mexErrMsgIdAndTxt("MATLAB:battery_model_mex:invalidNumInputs", 
                          "Cinco entradas são esperadas: i, V1, T, SOC, Nc.");
    }
    if (nlhs != 4) {
        mexErrMsgIdAndTxt("MATLAB:battery_model_mex:maxlhs", 
                          "Quatro saídas são esperadas: A, b, c, Vt.");
    }

    // Obtendo entradas do MATLAB
    double i = mxGetScalar(prhs[0]);
    double V1 = mxGetScalar(prhs[1]);
    double T = mxGetScalar(prhs[2]);
    double SOC = mxGetScalar(prhs[3]);
    double Nc = mxGetScalar(prhs[4]);

    // Criando saídas
    plhs[0] = mxCreateDoubleMatrix(2, 2, mxREAL); // A (2x2)
    plhs[1] = mxCreateDoubleMatrix(2, 1, mxREAL); // b (2x1)
    plhs[2] = mxCreateDoubleMatrix(1, 2, mxREAL); // c (1x2)
    plhs[3] = mxCreateDoubleScalar(0.0); // Vt (double)

    double *A_out = mxGetPr(plhs[0]);
    double *b_out = mxGetPr(plhs[1]);
    double *c_out = mxGetPr(plhs[2]);
    double *Vt_out = mxGetPr(plhs[3]);

    // Chamando a função battery_model
    battery_model(i, V1, T, SOC, Nc, (double (*)[2])A_out, 
                  (double (*)[1])b_out, (double (*)[2])c_out, Vt_out);
}