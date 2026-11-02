N = 20;
[X, Y] = meshgrid(linspace(-1, 1, N), linspace(-1, 1, N));
prescription_d_sphere = -3.5;
Phi_biometric = prescription_d_sphere * (X.^2 + Y.^2);
Phi_noise = 0.05 * sin(5*X) .* cos(5*Y);
Phi_Total = Phi_biometric + Phi_noise;
csvwrite('baseline_matrix.csv', Phi_total);
disp('Theoretical Wavefront Matrix successfully generated and exported.');

