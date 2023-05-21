#pragma once
#include "../TetrominoCor/TetrominoCor.h"
#include "../Dictionary/Dictionary.h"
#include <array>
const std::array<sf::Vector2f, 7> TetrominoWidth{ 
	[=]()
	{
		std::array<sf::Vector2f, 7> tW;
		for (size_t t = 0; t < 7; t++)
		{
			for (size_t i = 0; i < 3; i++)
			{
				sf::Vector2i s(kCell, kCell);
				for (size_t j = i + 1; j < 4; j++)
				{
					if ((TetrominoCor[t][i].y != TetrominoCor[t][j].y) &&
						(TetrominoCor[t][i].x == TetrominoCor[t][j].x))
						s.x = 0;
					if ((TetrominoCor[t][i].x != TetrominoCor[t][j].x) &&
						(TetrominoCor[t][i].y == TetrominoCor[t][j].y))
						s.y = 0;
				}
				tW[t].x += s.x; tW[t].y += s.y;
			}
			tW[t].x += kCell; tW[t].y += kCell;
		}
		return std::move(tW);
	}() };