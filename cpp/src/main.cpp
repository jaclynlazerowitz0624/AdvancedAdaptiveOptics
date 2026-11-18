#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "../include/MatrixLoader.hpp"
#include "../include/ColorMapper.hpp"


int main() {
	std::cout << "Initializing Advanced Adaptive Optics Engine..." << std::endl;

	const int gridSize = 32;
	sf::RenderWindow window (sf::VideoMode(800, 800), "Adaptive Optical Matrix Engine");
	window.setFramerateLimit(60);

	sf::Font font;
	bool fontLoaded = true;
	if (!font.loadFromFile("assets/font.ttf")) {
		std::cout << "Warning: Could not load font from assets/font.ttf. HUD text rendering disabled.\n";
		fontLoaded = false;
}

	//HUD text setup
	sf::Text hudText;
	if (fontLoaded) {
		hudText.setFont(font);
		hudText.setCharacterSize(24);
		hudText.setFillColor(sf::Color::White);
		hudText.setPosition(400.0f, 400.0f);
		}

	//FPS telemetry setup
	sf::Clock updateClock;
	int frameCount = 0;
	float currentFPS = 0.0f;

	std::string csvPath = "../matlab/baseline_matrix.csv";
	std::vector<std::vector<double>> wavefrontMatrix = MatrixLoader::loadCSV(csvPath, gridSize);

	float originX = 100.0f;
	float originY = 100.0f;

	//Slider UI state variables
	bool isDraggingSlider = false;
	float visualGain = 1.0f;
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
						sf::Vector2i localMouse = sf::Mouse::getPosition(window);
						float mouseX = static_cast<float>(localMouse.x);
						float mouseY = static_cast<float>(localMouse.y);

			if (mouseX >= sliderX - 20.0f && mouseX <= sliderX + sliderWidth + 20.0f && mouseY >= sliderY - 20.0f && mouseY <= sliderY + sliderHeight + 20.0f) {
				isDraggingSlider = true;
	}
}

} else if (event.type == sf::Event::MouseButtonReleased) {
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
						val *= 0.95;
				}
			}
				std::cout << "Executing Kalman state estimation closed-loop step...\n";
			}
			else if (event.key.code == sf::Keyboard::Up) {
				visualGain += 0.2f;
			}

			else if (event.key.code == sf::Keyboard::Down) {
				visualGain = std::max(0.1f, visualGain - 0.2f);
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

			else if (event.key.code == sf::Keyboard::R) {
				visualGain = 1.0f;
				std::cout << "Reset visual gain to default (1.0).\n";

			}


if (isDraggingSlider) {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i localMouse = sf::Mouse::getPosition(window);
		float mouseX = static_cast<float>(localMouse.x);
		float normalizedGain = (mouseX - sliderX) / sliderWidth;
		if (normalizedGain < 0.0f) normalizedGain = 0.0f;
		if (normalizedGain > 1.0f) normalizedGain = 1.0f;
		visualGain = 0.1f + normalizedGain * (5.0 - 0.1f);
	}
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
			cell.setFillColor(ColorMapper::valueToColor(val, minVal, maxVal));
	} else {
		cell.setFillColor(sf::Color(35, 35, 55));
}
	window.draw(cell);


		//Map visual gain to handle position
		float normalizedGain = (visualGain - 0.1f) / (5.0f - 0.1f);
		float handleX = sliderX + (normalizedGain * sliderWidth);

		//Draw slider background
		sf::RectangleShape sliderTrack (sf::Vector2f(sliderWidth, sliderHeight));
		sliderTrack.setPosition(sliderX, sliderY);
		sliderTrack.setFillColor(sf::Color(80, 80, 110));
		window.draw(sliderTrack);


		//Draw slider draggable handle
		sf::CircleShape sliderHandle(handleRadius);
		sliderHandle.setPosition(handleX - handleRadius, sliderY + (sliderHeight/2.0f) - handleRadius);
		sliderHandle.setFillColor(sf::Color(0, 220, 180));
		window.draw(sliderHandle);

		if (fontLoaded) {
			std::string telemetry = "Gain: " + std::to_string(visualGain).substr(0, 4) + " | FPS: " + std::to_string(static_cast<int>(currentFPS));
			hudText.setString(telemetry);
			window.draw(hudText);
		}

	window.display();
		}
	}
	return 0;
}
