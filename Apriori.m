function [X,P] = Apriori(A, B, i, X, Q, P)

    % EL FKE PROYECTO LOS ESTADOS POR DELANTE (A PRIORI)
    X  = (A * X) + (B * i);
    P  = (A * P * A') + Q; %ACTUALIZACION DEL ESTADO A PRIORI

end

