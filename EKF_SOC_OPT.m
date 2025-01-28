function [SOC_Estimated, Vt_Estimated, Vt_Error, P_history, KG_history] = EKF_SOC_OPT(I, V, Temp, Nc)

    load 'BattMod_T_1S_291024.mat'; %SE TIENE QUE CARGAR ASI PORQUE DA ERROR DE LA OTRA FORMA
    
    % MODIFICA PARA CARGAR LAS MATRICES DE COVARIANZA
    Matrices = 'C:\Users\leost\Downloads\EKF\Matrices_HPPC_20G1.mat';
    
    load (Matrices);
    
    SOC_t0 = 1;
    
    X = [SOC_t0;0];
    
    %INTERPOLACION PARA ESTIMAR VALORES INTERMEDIOS O DESCONOCIDOS EN FUNCION
    %DE LOS TADOS CONOCIDOS
    F_R0    = scatteredInterpolant(param(:,6),param(:,1),param(:,3)); % Temperatura, SoC, R0
    F_R1    = scatteredInterpolant(param(:,6),param(:,1),param(:,4)); % Temperatura, SoC, R1
    F_C1    = scatteredInterpolant(param(:,6),param(:,1),param(:,5)); % Temperatura, SoC, C1
    
    %CURVA SOC-OCV
    SOCOCV  = polyfit(param(:,1),param(:,2),6); % PARA ESTE CASO DE 4 EN ADELANTE TIENE UN COMPORTAMIENTO "BUENO"
    %DERIVADA DE LA CURVA OCV PARA LA MATRIZ C
    dSOCOCV = polyder(SOCOCV); %retorna una derivada de un polinomio
    
    %INICIO DE LOS VECTORES
    SOC_Estimated   = [];
    Vt_Estimated    = [];
    Vt_Error        = [];
    
    % Historial de P y KG para graficar posteriormente
    P_history = cell(length(I), 1);
    KG_history = cell(length(I), 1);
    
    % EL BUCLE RECORRE LA LONGITUD DE LOS DATOS EN FUNCION DE LA ENTRADA ACTUAL
    for k=1:1:length(I)
    
        T   = Temp(k); 
        i   = I(k); 
        Vcel = V(k); 
            
        SOC = X(1);
        V1  = X(2);
        
        [A,B,C,Vt] = Model(F_R0,F_R1,F_C1,dSOCOCV,SOCOCV,i,V1,T,SOC,Nc);
    
        % CALCULO DEL ERROR 
        Error_x   = Vcel - Vt;
        
    
        [X, P] = Apriori(A, B, i, X, Q, P);
        [X, P, KG] = Aposteriori(X, P, C, R, Error_x);
    
        % ACTUALIZACION DEL SOC Y GUARDAR EN LOS VECTORES
        Vt_Estimated(k)  = Vt;
        SOC_Estimated(k) = X(1);
        Vt_Error(k)      = Error_x;
    
        % Guardar \(P\) y \(K_G\) en el historial
        P_history{k} = P;
        KG_history{k} = KG;
        

    end

    SOC_Estimated=SOC_Estimated';
    Vt_Estimated=Vt_Estimated';
    Vt_Error=Vt_Error';
end

    
