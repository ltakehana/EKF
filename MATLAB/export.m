clear;
clc;

load("EKF_HPPC_20.mat");

Ah(1:end)=Nc;

data_table = table(t, Ah, I, SoC_CCM, T, V, 'VariableNames', {'t', 'Ah', 'I', 'SoC_CCM', 'T', 'V'});

writetable(data_table, 'dados.csv');