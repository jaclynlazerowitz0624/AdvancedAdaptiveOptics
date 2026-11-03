#include <SFML/Graphics.hpp>
#include "../include/MatrixLoader.hpp"
#include <iostream>

int main() {
	std::cout << "Initializing Advanced Adaptive Optics Engine..." << std::endl;

	//Load the baseline wavefront matrix from MATLAB
	//(Looks back from the build folder to access MATLAB)
	auto wavefrontMatrix = MatrixLoader::loadCSV("../matlab/baseline_matrix.csv", 20);

	//Accesses and renders the SFML window
	sf::REnderWindow window(sf::VideoMode(800, 600), "Advance Adaptive Optics Telemetry & Control Engine");
	window.setFramerateLimit(60);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type==sf::Event::Closed)
				window.close();

		}

		window.clear(sf::Color(15, 15, 25)); //Dark theme
		//TODO: Render wavefront mapping and interactive UI sliders later in the program
		window.display();
	}
	return 0;
}
