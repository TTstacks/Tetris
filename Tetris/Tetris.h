#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include "../TetrominoCor/TetrominoCor.h"
#include "../Dictionary/Dictionary.h"
#include "../TetrominoColor/TetrominoColor.h"
#include "../TetrominoWidth/TetrominoWidth.h"
#include "../TetrisMap/TetrisMap.h"
class Tetris
{
public:
	Tetris();
private:
	void Update();
	void Rotate();
	bool Check();
	bool GameOver();
	void HandleEvent();
	void Init();
	void InitNextTetromino();
	void InitCurTetromino();
	void InitGhostTetromino();
	void Draw();
	void DrawMap();
	void DrawCurTetromino();
	void DrawNextTetromino();
	void DrawGhostTetromino();
	void ClearRow();
private:
	std::array<sf::Vector2i, 4> nextTetromino;
	std::array<sf::Vector2i, 4> curTetromino;
	std::array<sf::Vector2i, 4> prevTetromino;
	std::array<sf::Vector2i, 4> ghostTetromino;
	std::array<sf::Vertex, 4> rectangle;
	sf::Vector2f tWidth;
	int dropDistance;
	size_t curColorIndex;
	size_t nextColorIndex;
	sf::RenderWindow window;
	sf::Clock clock;
	int dX;
	float delta;
};