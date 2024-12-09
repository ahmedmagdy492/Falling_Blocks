#pragma once

#include "Tetrominos.h"

class Game {
private:
	bool isPlaying;
	Tetrominos* activeTetrominos;

	ShapeType GenerateNextTetrominos();

public:
	Game();

	void StartGame();
	bool GetGameState();
	void EndGame();

	void DrawActiveTet();
	void RotateActiveTet();
	void MoveActiveTet(BlocksMoveDirection dir);

	~Game();
};