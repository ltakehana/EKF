function [X, P, KG] = Aposteriori(X, P, C, R,Error_x)
  
    % GANANCIA DE KALMAN - ACTUALIZACION DE LA ESTIMACION CON LA MEDIDA (A POSTERIORI)
    KG  = (P) * (C') * (inv((C * P * C') + (R)));
    %  Update the estimate with measurement zk (a-posteriori):
    X  = X + KG * Error_x;
    % Update the error covariance   :
    P = (eye(size(X, 1)) - KG * C) * P;
end