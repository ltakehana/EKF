clc; clear; close all;
Archivo = '/home/leonardo/EKF/EKF_HPPC_20.mat'; %EKF_T2_20

load(Archivo);


%ACTIVAR PARA UNA SOLA BATERIA
I = I(:,1);
T = T(:,1);
V = V(:,1);

 I_mod = I;


%Nc          = 1.5629; % Battery capacity in Ah taken from data
Me_SOC      = ((Nc + Ah)*100)/Nc; %Calcule el SOC usando Coloumb Counting para la comparación

% EL TIEMPO SE PASA DE SEGUNDOS A HORAS 
Time_Hours  = t/3600;

%Cronometro
tic;
% FUNCION PARA FILTRO DE KALMAN METODO CHYNTIA

% FUNCION PARA FILTRO DE KALMAN METODO OPTIMIZACION CAMILO
[SOC_Estimated, Vt_Estimated, Vt_Error, P_history, KG_history] = EKF_SOC_OPT(I_mod, V, T, Nc);

tiempo_ejecucion = toc; % Detiene el cronómetro y obtiene el tiempo transcurrido
disp(['Tempo de execução do EKF_SOC_OPT: ', num2str(tiempo_ejecucion), ' segundos']);

% Crear una nueva figura
figure;
% Primer subplot: Voltaje terminal medido vs. estimado
subplot(2,2,1);
plot(Time_Hours,V);
hold on;
plot(Time_Hours,Vt_Estimated);
hold off;
legend('Medição','Estimativa EKF');
ylabel('Tensão do terminal [V]');
xlabel('Tempo [Horas]');
title('Medição VS Estimativa da tensão do terminal (V) (20°C) '); %por 0 Deg C
grid minor;

% Segundo subplot: Terminal Voltage de Error
subplot(2,2,2);
plot(Time_Hours,Vt_Error);
legend('Erro no terminal de tensão');
ylabel('Erro no terminal de tensão');
xlabel('Tempo [Horas]');

% Tercer subplot: SOC Coulomb Counting vs. Estimacion
subplot(2,2,3);
plot(Time_Hours,Me_SOC);
hold on;
plot(Time_Hours,SOC_Estimated*100);
hold off;
legend('Coulomb Counting','Estimativa EKF');
ylabel('SOC [%]');
xlabel('Tempo [Horas]');
title('Coulomb Counting VS SOC Estimativa (20°C)'); %por 0 Deg C
grid minor;

% Cuarto subplot: SOC Error
subplot(2,2,4);
plot(Time_Hours,(Me_SOC - SOC_Estimated*100));
legend('Erro do SOC');
ylabel('Erro do SOC [%]');
xlabel('Tempo [Horas]');
grid minor

for i = 1:size(KG_history,1)
KG(1,i) = KG_history{i}(1);
KG(2,i) = KG_history{i}(2);
P(1,i) = P_history{i}(1);
P(2,i) = P_history{i}(4);
end


%% Calcular RMSE para el voltaje terminal
RMSE_Vt = sqrt(mean((V - Vt_Estimated).^2));
disp(['RMSE para tensão terminal: ', num2str(RMSE_Vt), ' V']);

% Calcular RMSE para el estado de carga (SOC)
% Comparar Me_SOC (Coulomb Counting) con SOC_Estimated (de EKF)
RMSE_SOC = sqrt(mean((Me_SOC - SOC_Estimated*100).^2));
disp(['RMSE para o estado de carga: ', num2str(RMSE_SOC), ' %']);

TempAc = 20;
RMSE = table(TempAc, RMSE_SOC, RMSE_Vt, tiempo_ejecucion, 'VariableNames', {'Temperatura', 'RMSE_SOC', 'RMSE_Vt', 'Tempo de execução'});
save('RMSE_T1_20G_ruidomediazero.mat', 'RMSE');

