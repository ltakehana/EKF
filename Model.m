function [A,B,C,Vt] = Model(F_R0,F_R1,F_C1,dSOCOCV,SOCOCV,i,V1,T,SOC,Nc)
    
    DeltaT = 1;
    Qn   = Nc * 3600; %% PILAS, DEBE SER CAMBIADO

    % SE EVALUAN LOS PARAMETROS DE LA BATERIA EN FUNCION DE LA TEMPERATURA Y SOC ACTUAL 
    R0  = F_R0(T,SOC);
    R1  = F_R1(T,SOC);  
    C1  = F_C1(T,SOC);

    % CALCULA LOS VALORES DE VOC EN UN SOC DADO
    Voc = polyval(SOCOCV,SOC);  % Voc = b0 + b1 *SoC en trabajo Cynthia

    U1  = C1 * R1;
    a1  = exp(-DeltaT/U1);
    b1  = R1 * (1 - exp(-DeltaT/U1));

    Vt  = Voc - R0*i - V1; 

    if i > 0
        eta = 1; % DESCARGA
    elseif i <= 0 
        eta = 0.998; % CARGA
    end

    % LINEALIZADION DE LA MATRIZ C
    dOCV = polyval(dSOCOCV, SOC);
    C    = [dOCV -1]; 

    A   = [1 0 ;0 a1];
    B   = [-(eta * DeltaT/Qn); b1];
end

