gridSize = 20;
[X, Y] = meshgrid(linspace(-1, 1, gridSize));
astigmatismMatrix = 2.5 * (X.^2 - Y.^2) + 0.5 * sin(pi * X);
csvwrite('astigmatism_matrix.csv', astigmatismMatrix);
disp('Astigmatism wavefront matrix generated and exported successfully.');
