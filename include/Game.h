#pragma once

#include "Tetrominos.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
#include <set>

enum GameState {
	Playing,
	GameOver,
	Pause
};

class Game {
private:
	static const int soundsCount = 3;

	GameState curGameState = GameState::Pause;
	Tetromino* activeTetromino;
	ShapeType nextTetromino;
	std::vector<Vector2> squares;
	std::list<std::pair<float, float>> squaresToRemove;
	Color defaultSquaresColor = DARKGRAY;
	unsigned int playerScore = 0;
	Sound sounds[soundsCount];

	ShapeType GenerateNextTetromino();

	Tetromino* CreateTetromino(ShapeType shapeType);
	void OnTetrominoTouchesGround();

	inline Color GetColorBasedOnShapeType(ShapeType shapeType) const {
		switch (shapeType) {
		case ShapeType::IShape:
			return SKYBLUE;
		case ShapeType::JShape:
			return BLUE;
		case ShapeType::LShape:
			return ORANGE;
		case ShapeType::OShape:
			return YELLOW;
		case ShapeType::SShape:
			return DARKGREEN;
		case ShapeType::ZShape:
			return RED;
		}

		return MAGENTA;
	}

public:
	Game();

	void StartGame();
	void EndGame();

	inline unsigned int GetPlayerScore() const {
		return playerScore;
	}

	inline void SetGameState(GameState gameState) {
		curGameState = gameState;
	}

	inline GameState GetGameState() const {
		return curGameState;
	}

	inline unsigned int FindValueInList(std::vector<Vector2>& list, Vector2 value) {
		int i = 0;
		for (Vector2& vec : list) {
			if (vec.x == value.x && vec.y == value.y) {
				return i;
			}
			++i;
		}

		return -1;
	}

	inline void IncreamentScore(unsigned char noClearedLines) {
		switch (noClearedLines) {
		case 1:
			playerScore += Constants::singleScoreValue;
			break;
		case 2:
			playerScore += Constants::doubleScoreValue;
			break;
		case 3:
			playerScore += Constants::tripleScoreValue;
			break;
		default:
			playerScore += Constants::tetrisScoreValue;
			break;
		}
	}

	void ClearCompeleteLine(std::set<float>& xValues, float maxYValue);
	void ClearCompletedLineIfThereAny();
	void DrawActiveTet();
	void DrawNextTet();
	void DrawSquares();
	void RotateActiveTet();
	void MoveActiveTet(BlocksMoveDirection dir);
	unsigned int GetTetWidth(ShapeType type);
	void ResetGame();

	~Game();
};