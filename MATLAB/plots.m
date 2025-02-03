% Definição do espaço de variáveis
T = -5:1:50;         % Temperatura de -5 a 50 com passo 1
SOC = 0:0.01:1;      % Estado de carga de 0 a 1 com passo 0.02

% Criação das malhas de T e SOC
[T_grid, SOC_grid] = meshgrid(T, SOC);

% Avaliação das funções interpolantes
R0 = F_R0(T_grid, SOC_grid);
R1 = F_R1(T_grid, SOC_grid);
C1 = F_C1(T_grid, SOC_grid);

% Avaliação das funções polinomiais
Voc = polyval(SOCOCV, SOC);
dOCV = polyval(dSOCOCV, SOC);

% Plot das superfícies das funções interpoladas
figure;
subplot(2,2,1);
surf(T_grid, SOC_grid, R0);
xlabel('Temperatura (°C)'); ylabel('SOC'); zlabel('R0');
title('R0(T, SOC)'); shading interp; colorbar;

subplot(2,2,2);
surf(T_grid, SOC_grid, R1);
xlabel('Temperatura (°C)'); ylabel('SOC'); zlabel('R1');
title('R1(T, SOC)'); shading interp; colorbar;

subplot(2,2,3);
surf(T_grid, SOC_grid, C1);
xlabel('Temperatura (°C)'); ylabel('SOC'); zlabel('C1');
title('C1(T, SOC)'); shading interp; colorbar;

% Plot das curvas polinomiais
figure;
subplot(2,1,1);
plot(SOC, Voc, 'b', 'LineWidth', 2);
xlabel('SOC'); ylabel('Voc (V)');
title('Open Circuit Voltage (Voc)');

subplot(2,1,2);
plot(SOC, dOCV, 'r', 'LineWidth', 2);
xlabel('SOC'); ylabel('dOCV/dSOC (V)');
title('Derivada da OCV (dOCV/dSOC)');

grid on;
