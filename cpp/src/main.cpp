#include <SFML/Graphics.hpp>
#include "../include/MatrixLoader.hpp"
#include <iostream>
#include <algorithm>


//Function tha maps scalar values to a color gradient (Blue -> Green -> Red)
sf::Color valueToColor(double val, double minVal, double maxVal) {
	//Normalize value between 0.0 and 1.0 (makes the program easier to compile and run since a simple number is used)
	double normalized = (maxVal - minVal == 0) ? 0.0 : (val - minVal) / (maxVal - minVal);
	normalized = std::clamp(normalized, 0.0, 1.0);

	//Colormap mapping using the simplified "normalized" values from above
	sf::Uint8 r = static_cast<sf::Uint8>(255 * normalized);
	sf::Uint8 g = static_cast<sf::Uint8>(255 * (1.0 - std::abs(normalized - 0.5) * 2.0));
	sf::Uint8 b = static_cast<sf::Uint8>(255 * (1.0 - normalized));

	return sf::Color(r, g, b);

}


int main() {
	std::cout << "Initializing Advanced Adaptive Optics Engine..." << std::endl;

	//Load the baseline wavefront matrix from MATLAB and declared gridSize variable and defines it
	//(Looks back from the build folder to access MATLAB)
	int gridSize = 20;
	auto wavefrontMatrix = MatrixLoader::loadCSV("../matlab/baseline_matrix.csv", 20);

	//Finds min and max values in the matrix for coloring and color normalization
	double minVal = 1e9, maxVal = -1e9;
	for (const auto& row: wavefrontMatrix) {
		for (double val : row) {
			if (val < minVal) minVal = val;
			if (val > maxVal) maxVal = val;
	}
}

	//Accesses and renders the SFML window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Advance Adaptive Optics Telemetry & Control Engine");
	window.setFramerateLimit(60);

	//UI Grid sizing controls
	float cellSize = 22.0f;
	float originX = 100.0f;
	float originY = 100.0f;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type==sf::Event::Closed)
				window.close();

		}

		window.clear(sf::Color(15, 15, 25)); //Dark theme

	//REnders wavefront heatmap grid
	for (int i = 0; i < gridSize; ++i) {
		for (int j = 0; j < gridSize; ++j) {
			sf::RectangleShape cell(sf::Vector2f(cellSize - 2.0f, cellSize - 2.0f));
			cell.setPosition(originX + j * cellSize, originY + i * cellSize);

	//Colors cells based on refraction value of the matrix
	cell.setFillColor(valueToColor(wavefrontMatrix[i][j], minVal, maxVal));
	window.draw(cell);

	}
}

		//TODO: Render wavefront mapping and interactive UI sliders later in the program
		window.display();
	}
	return 0;
}
