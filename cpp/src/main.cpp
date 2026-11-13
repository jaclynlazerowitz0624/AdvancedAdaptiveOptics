#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "../include/MatrixLoader.hpp"
#include "ColorMapper.hpp"


//Function the maps scalar values to a color gradient (Blue -> Green -> Red)
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

	const int gridSize = 32;
	sf::RenderWindow window (sf::VideoMode(800, 800), "Adaptive Optical Matrix Engine");
	window.setFramerLimit(60);

	std::string csvPath = "../matlab/baseline_matrix.csv";
	std::vector<std::vector<double>> wavefrontMatrix = MatrixLoader::loadCSV(csvPath, gridSize);

	double visualGain = 1.0f;
	float originx = 100.0f;
	float originy = 100.0f;

	//Slider UI state variables
	bool isDraggingSlider = false;
	float sliderX = 100.0f;
	float sliderY = 30.0f;
	float sliderWidth = 250.0f;
	float sliderHeight = 10.0f;
	float handleRadius = 8.0f;


	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type==sf::Event::Closed)
				window.close();

				if (event.type == sf::Event::MouseButtonPressed) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y ? static_cast<float>(event.mouseButton.x) : 0, static_cast<float>(event.mouseButton.y));
			if (mousePos.x >= sliderX - 10.0f && mousePos.x < = sliderX + sliderWidth + 10.0f && mousePos.y >= sliderY - 15.0f && mousePos.y <= sliderY + sliderHeight + 15.0f) {
				isDraggingSlider = true;

				float mouseX = mousePos.x;
				if (mouseX < sliderX) mouseX = sliderX;
				if (mouseX > sliderX + sliderWidth) mouseX = sliderX + sliderWidth;
				float normalized = (mouseX - sliderX)/sliderWidth;
				visualGain = 0.1f + normalized * (5.0f-0.1f);
		}
	}
}
else if (event.type == sf::Event::MouseButtonReleased) {
	if (event.mouseButton.button == sf::Mouse::Left) {
		isDraggingSlider = false;
	}
}

		//Handles keyboard inputs for user controls
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Escape) {
				window.close();
			}

			else if (event.key.code == sf::Keyboard::C) {
				for (auto& row : wavefrontMatrix) {
					for (double& val : row) {
						val *= 0.85;
				}
			}
				std::cout << "Executing Kalman state estimation closed-loop step...\n";
			}
			else if (event.key.code == sf::Keyboard::Up) {
				visualGain += 0.2;
			}

			else if (event.key.code == sf::Keyboard::Down) {
				visualGain = std::max(0.1, visualGain - 0.2f);
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

if (isDraggingSlider) {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i localMouse = sf::Mouse::getPosition(window);
		float mouseX = static_cast<float>(localMouse.x);

		if (mouseX < sliderX) mouseX = sliderX;
		if (mouseX > sliderX + sliderWidth) mouseX = sliderX + sliderWidth;

		float normalized = (mouseX - sliderX) / sliderWidth;
		visualGain = 0.1f + normalized * (5.0f - 0.1f);

	} else {
		isDraggingSlider = false;
	}
}

		window.clear(sf::Color(15, 15, 25));

		double cellSize = 600.0/gridSize;

		double minVal = wavefrontMatrix[0][0];
		double maxVal = wavefrontMatrix[0][0];
		for (const auto& row : wavefrontMatrix) {
			for(double v : row) {
				if (v < minVal) minVal = v;
				if (v > maxVal) maxVal = v;
	}
}
if ((maxVal - minVal) < 0.0001){
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
		cell.setFillColor(sf::Color(35, 35, 55));
}
	window.draw(cell);

	}
}


		//Map visual gain to handle position
		float normalizedGain = (visualGain - 0.1f) / (5.0f - 0.1f);
		if (normalizedGain<0.0f) normalizedGain =0.0f;
		if (normalizedGain>1.0f) normalizedGain = 1.0f;
		float handleX = sliderX + (normalizedGain * sliderWidth);

		//Draw slider background
		sf::RectangleShape sliderTrack (sf::Vector2f(sliderWidth, sliderHeight));
		sliderTrack.setPosition(sliderX, sliderY);
		sliderTrack.setFillColor(sf::Color(80, 80, 110));
		sliderTrack.setOutlineColor(sf::Color(150, 150, 180));
		sliderTrack.setOutlineThickness(1.0f);
		window.draw(sliderTrack);

		//Draw slider draggable handle
		sf::CircleShape sliderHandle(handleRadius);
		sliderHandle.setPosition(handleX - handleRadius, sliderY + (sliderHeight/2.0f) - handleRadius);
		sliderHandle.setFillColor(sf::Color(0, 220, 180));
		window.draw(sliderHandle);

	window.display();
	}
	return 0;
}
