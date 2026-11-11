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

	//Kalman filter Estimation Parameters
        double processNoise = 0.01; //= the unsteadiness in liquid crystals
        double measurementNoise = 0.08; //= sensor noise from phase detection
        double errorCovariance = 1.0; //= initial estimated uncertainty

	//Load the baseline wavefront matrix from MATLAB and declared gridSize variable and defines it
	//(Looks back from the build folder to access MATLAB)
	int gridSize = 20;
	std::string csvPath = "../matlab/baseline_matrix.csv";
	auto wavefrontMatrix = MatrixLoader::loadCSV(csvPath, gridSize);

	//Finds min and max values in the matrix for coloring and color normalization
	double minVal = -2.0, maxVal = 2.0;
	double visualGain = 1.0;

	//Accesses and renders the SFML window
	sf::RenderWindow window(sf::VideoMode(950, 750), "Advance Adaptive Optics Telemetry & Control Engine");
	window.setFramerateLimit(60);

	sf::Font font;
	bool fontLoaded = font.loadFromFile("../assets/font.ttf");
	if (!fontLoaded) {
		std::cout << "Warning: Could not load font from ../assests/font.ttf. HUD text rendering disabled." << std::endl;
}

	sf::Text telemetryText;
	if (fontLoaded) {
		telemetryText.setFont(font);
		telemetryText.setCharacterSize(16);
		telemetryText.setFillColor(sf::Color::White);
		telemetryText.setPosition(50.0f, 30.0f);
}

	//UI Grid sizing controls
	float cellSize = 22.0f;
	float originX = 100.0f;
	float originY = 100.0f;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type==sf::Event::Closed)
				window.close();

		//Handles keyboard inputs for user controls
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::R) {
				std::cout << "Reloading wavefront matrix from disk..." << std::endl;
				wavefrontMatrix = MatrixLoader::loadCSV(csvPath, gridSize);
			}

			else if (event.key.code == sf::Keyboard::C) {
				std::cout << "Executing Kalman state estimation closed-loop step...\n";

				double predictedCovariance = errorCovariance + processNoise;
				double kalmanGain = predictedCovariance / (predictedCovariance+measurementNoise);

				for (int i = 0; i < gridSize; ++i) {
					for (int j = 0; j < gridSize; ++j) {
						double measurement = wavefrontMatrix[i][j];
						double residual = 0.0-measurement;

						wavefrontMatrix[i][j] += kalmanGain* residual;
			}
	}

	errorCovariance = (1.0 - kalmanGain)*predictedCovariance;
	std::cout << "Updated Kalman Gain: " << kalmanGain
		<< " | Error Covariance: " << errorCovariance << "\n";
}
			else if (event.key.code == sf::Keyboard::Up) {
				visualGain += 0.1;
				std::cout << "Increased visual gain multiplier to: " << visualGain << std::endl;
			}

			else if (event.key.code == sf::Keyboard::Down) {
				visualGain = std::max(0.1, visualGain - 0.1);
				std::cout << "Decreased visual gain multiplier to: " << visualGain << std::endl;
			}

			else if (event.key.code == sf::Keyboard::Left) {
				csvPath = "../matlab/baseline_matrix.csv";
				wavefrontMatrix = MatrixLoader::loadCSV(csvPath, gridSize);
				std::cout << "Switched to baseline wavefront profile." << std::endl;

			}

			else if (event.key.code == sf::Keyboard::Right) {
				csvPath = "../matlab/astigmatism_matrix.csv";
				wavefrontMatrix = MatrixLoader::loadCSV(csvPath, gridSize);
				std::cout << "Switched to astigmatism wavefront profile." << std::endl;
}

		}
}

		window.clear(sf::Color(15, 15, 25));

		double cellSize = 800.0/gridSize;

		double minVal = wavefrontMatrix[0][0];
		double maxVal = wavefrontMatrix[0][0];
		for (const auto& row : wavefrontMatrix) {
			for(double v : row) {
				if (v < minVal) minVal = v;
				if (v > maxVal) maxVal = v;
}
}
if (maxVal - minVal < 0.0001){
	 minVal -=0.5;
	maxVal +=0.5;
}
		double centerX = gridSize/2.0;
		double centerY = gridSize/2.0;
		double radius = gridSize/2.0;

	//REnders wavefront heatmap grid
	for (int i = 0; i < gridSize; ++i) {
		for (int j = 0; j < gridSize; ++j) {
			sf::RectangleShape cell(sf::Vector2f(cellSize - 2.0f, cellSize - 2.0f));
			cell.setPosition(originX + j * cellSize, originY + i * cellSize);

		double dx = j - centerX + 0.5;
		double dy = i - centerY + 0.5;
		if ((dx * dx + dy * dy) <= (radius*radius)) {
			double val = wavefrontMatrix[i][j] * visualGain;
			cell.setFillColor(valueToColor(val, minVal, maxVal));
	} else {
		cell.setFillColor(sf::Color(10, 10, 15));
}
	window.draw(cell);

	}
}

		//TODO: Render wavefront mapping and interactive UI sliders later in the program
		window.display();
	}
	return 0;
}
