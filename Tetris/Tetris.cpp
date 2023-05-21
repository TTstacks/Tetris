#include "Tetris.h"
#include <algorithm>
#include <iostream>

Tetris::Tetris()
	: dX(0), delta(0.4f), dropDistance(kArrH - 1)
{
	srand(static_cast<unsigned int>(time(nullptr)));
	window.create({ kScreenW * 2, kScreenH }, "Tetris");
	window.setFramerateLimit(60);
	InitNextTetromino();
	Init();
	Update();
}

bool Tetris::Check()
{
	return std::any_of(curTetromino.begin(), curTetromino.end(), 
		[=](const sf::Vector2i& val) {
			return val.x < 0 || val.x >= kArrW || val.y >= kArrH || TetrisMap[GetIndex(val.x, val.y)];
		});
}

bool Tetris::GameOver()
{
	return !dropDistance;
}

void Tetris::HandleEvent()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed: window.close(); return;
		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::Left: dX = -1; return;
			case sf::Keyboard::Right: dX = 1; return;
			case sf::Keyboard::Up: Rotate(); return;
			case sf::Keyboard::S: curTetromino = ghostTetromino; return;
			default: return;
			}
			return;
		default: return;
		}
	}
}

void Tetris::Rotate()
{
	const auto& [pX, pY] = curTetromino[0];
	for (auto& [x, y] : curTetromino)
	{
		const auto x1 = x;
		x = y + pX - pY;
		y = pX + pY - x1;
	}
	if (Check()) curTetromino = prevTetromino;
}

void Tetris::Init()
{
	InitCurTetromino();
	InitNextTetromino();
}

void Tetris::InitNextTetromino()
{
	const size_t& tetrominoIndex = rand() % kTetrominos;
	tWidth = TetrominoWidth[tetrominoIndex];
	nextColorIndex = static_cast<size_t>(rand()) % kColors + 1;
	for (size_t i = 0; i < 4; i++)
	{
		nextTetromino[i].x = TetrominoCor[tetrominoIndex][i].x;
		nextTetromino[i].y = TetrominoCor[tetrominoIndex][i].y;
	}
}

void Tetris::InitCurTetromino()
{
	curColorIndex = nextColorIndex;
	const int& xPos = rand() % (kArrW - 3);
	for (size_t i = 0; i < 4; i++)
	{
		curTetromino[i].x = nextTetromino[i].x + xPos;
		curTetromino[i].y = nextTetromino[i].y;
	}
}

void Tetris::InitGhostTetromino()
{
	const int& maxY = std::max({ curTetromino[0].y, curTetromino[1].y, curTetromino[2].y, curTetromino[3].y });
	int d = dropDistance - maxY;
	if (dropDistance < kArrH - 1)
	{
		bool check = true;
		while (check)
		{
			for (const auto& [x, y] : curTetromino)
			{
				if (static_cast<size_t>(d) + y == kArrH - 1) { check = false; break; }
				if (TetrisMap[GetIndex<int>(x, y + d + 1)]) { check = false; break; }
			}
			d++;
		}
		d--;
	}
	for (size_t i = 0; i < 4; i++)
	{
		ghostTetromino[i].x = curTetromino[i].x;
		ghostTetromino[i].y = curTetromino[i].y + d;
	}
}

void Tetris::Update()
{
	while (window.isOpen())
	{
		HandleEvent();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) delta = 0.05f;
		for (size_t i = 0; i < 4; i++)
		{
			prevTetromino[i] = curTetromino[i];
			curTetromino[i].x += dX;
		}
		if (Check()) curTetromino = prevTetromino;
		if (clock.getElapsedTime().asSeconds() > delta)
		{
			for (size_t i = 0; i < 4; i++)
			{
				prevTetromino[i] = curTetromino[i];
				curTetromino[i].y++;
			}
			if (Check())
			{
				for (const auto& [x, y] : prevTetromino) TetrisMap[GetIndex(x, y)] = curColorIndex;
				Init();
			}
			clock.restart();
		}
		InitGhostTetromino();
		if (GameOver()) TetrisMap.fill(0);
		ClearRow();
		Draw();
		dX = 0;
		delta = 0.4f;
	}
}

void Tetris::Draw()
{
	window.clear();
	DrawMap();
	DrawCurTetromino();
	DrawGhostTetromino();
	DrawNextTetromino();
	window.display();
}

void Tetris::DrawMap()
{
	rectangle[0].position = { 0.f, 0.f };
	rectangle[1].position = { 0.f, static_cast<float>(kScreenH) };
	rectangle[2].position = { static_cast<float>(kScreenW), static_cast<float>(kScreenH) };
	rectangle[3].position = { static_cast<float>(kScreenW), static_cast<float>(0.f) };
	rectangle[0].color = rectangle[1].color = rectangle[2].color = rectangle[3].color = sf::Color(32, 38, 47);
	window.draw(rectangle.data(), 4, sf::Quads);
	for (size_t i = 0; i < kArrH; i++)
		for (size_t j = 0; j < kArrW; j++)
		{
			rectangle[0].position = { static_cast<float>(j * kCell + 1), static_cast<float>(i * kCell + 1) };
			rectangle[1].position = { static_cast<float>(j * kCell + 1), static_cast<float>(i * kCell + kCell - 1) };
			rectangle[2].position = { static_cast<float>(j * kCell + kCell - 1), static_cast<float>(i * kCell + kCell - 1) };
			rectangle[3].position = { static_cast<float>(j * kCell + kCell - 1), static_cast<float>(i * kCell + 1) };
			for(auto& rec : rectangle) rec.color = TetrominoColor[TetrisMap[GetIndex(j, i)]];
			window.draw(rectangle.data(), 4, sf::Quads);
		}
}

void Tetris::DrawCurTetromino()
{
	for(auto& rec : rectangle) rec.color = TetrominoColor[curColorIndex];
	for (const auto& [x, y] : curTetromino)
	{
		rectangle[0].position = { static_cast<float>(x * kCell + 1), static_cast<float>(y * kCell + 1) };
		rectangle[1].position = { static_cast<float>(x * kCell + 1), static_cast<float>(y * kCell + kCell - 1) };
		rectangle[2].position = { static_cast<float>(x * kCell + kCell - 1), static_cast<float>(y * kCell + kCell - 1) };
		rectangle[3].position = { static_cast<float>(x * kCell + kCell - 1), static_cast<float>(y * kCell + 1) };
		window.draw(rectangle.data(), 4, sf::Quads);
	}
}

void Tetris::DrawNextTetromino()
{
	sf::RectangleShape s({150, 100});
	s.setPosition({kScreenW / 2 - s.getSize().x / 2 + kScreenW, 70});
	s.setFillColor(sf::Color(8, 14, 24));
	s.setOutlineColor(sf::Color(230, 230, 231));
	s.setOutlineThickness(2.f);
	const float& yPos = s.getPosition().y + (s.getSize().y / 2 - tWidth.y / 2);
	const float& xPos = s.getSize().x / 2 - tWidth.x / 2 + s.getPosition().x;
	window.draw(std::move(s));
	for(auto& rec : rectangle) rec.color = TetrominoColor[nextColorIndex];
	for (const auto& [x, y] : nextTetromino)
	{
		rectangle[0].position = { static_cast<float>(x * kCell + xPos), static_cast<float>(y * kCell + yPos) };
		rectangle[1].position = { static_cast<float>(x * kCell + xPos), static_cast<float>(y * kCell + kCell + yPos) };
		rectangle[2].position = { static_cast<float>(x * kCell + kCell + xPos), static_cast<float>(y * kCell + kCell + yPos) };
		rectangle[3].position = { static_cast<float>(x * kCell + kCell + xPos), static_cast<float>(y * kCell + yPos) };
		window.draw(rectangle.data(), 4, sf::Quads);
	}
}

void Tetris::DrawGhostTetromino()
{
	for (auto& rec : rectangle) rec.color.a = 70;
	for (const auto& [x, y] : ghostTetromino)
	{
		rectangle[0].position = { static_cast<float>(x * kCell + 1), static_cast<float>(y * kCell + 1) };
		rectangle[1].position = { static_cast<float>(x * kCell + 1), static_cast<float>(y * kCell + kCell - 1) };
		rectangle[2].position = { static_cast<float>(x * kCell + kCell - 1), static_cast<float>(y * kCell + kCell - 1) };
		rectangle[3].position = { static_cast<float>(x * kCell + kCell - 1), static_cast<float>(y * kCell + 1) };
		window.draw(rectangle.data(), 4, sf::Quads);
	}
	for (auto& rec : rectangle) rec.color.a = 255;
}

void Tetris::ClearRow()
{
	int col = kArrH - 1;
	bool initialized = false;
	for (ptrdiff_t i = kArrH - 1; i >= 0; i--)
	{
		size_t count = 0;
		for (size_t j = 0; j < kArrW; j++)
		{
			if (TetrisMap[GetIndex<size_t>(j, i)]) count++;
			TetrisMap[GetIndex<size_t>(j, col)] = TetrisMap[GetIndex<size_t>(j, i)];
		}
		if (count < kArrW) col--;
		if (!count && !initialized) { dropDistance = i; initialized = true; }
	}
}