#pragma once
#include <SFML/Graphics.hpp>
#include <array>
const std::array<std::array<sf::Vector2u, 4>, 7> TetrominoCor{
	{
		{{{1, 0}, {0, 0}, {2, 0}, {3, 0}}},
		{{{1, 0}, {0, 0}, {2, 0}, {2, 1}}},
		{{{1, 1}, {0, 0}, {2, 1}, {1, 0}}},
		{{{1, 0}, {0, 0}, {2, 0}, {1, 1}}},
		{{{1, 1}, {0, 1}, {1, 0}, {2, 0}}},
		{{{1, 0}, {0, 0}, {2, 0}, {0, 1}}},
		{{{1, 1}, {0, 0}, {1, 0}, {0, 1}}},
	}
};