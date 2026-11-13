#ifndef COLOR_MAPPER_HPP
#define COLOR_MAPPER_HPP

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>

inline sf::Color valueToColor(double val, double minVal, double maxVal){
	if (maxVal - minVal < 0.00001) {
		return sf::Color (0, 255, 0); //prevents division by 0
}


//Normalize value between 0.0 and 1.0
double normalized = (val-minVal) / (maxVal - minVal);
normalized = std::clamp(normalized, 0.0, 1.0);

//Thermal color mapping [blue->cyan->green->yellow->red]
int r_val = static_cast<int>(std::max(0.0, std::min(1.0, r_val)) * 255);
int g_val = static_cast<int>(std::max(0.0, std::min(1.0, g_val)) * 255);
int b_val = static_cast<int>(std::max(0.0, std::min(1.0, b_val)) * 255);

return sf::Color(static_cast<sf::Uint8>(r), static_cast<sf::Uint8>(g), static_cast<sf::Uint8>(b));

}

#endif // COLOR_MAPPER_HPP
