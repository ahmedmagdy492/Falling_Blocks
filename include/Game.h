#pragma once

#include "Tetrominos.h"

#include <iostream>
#include <vector>
#include <map>
#include <set>

class Game {
private:
	bool isPlaying;
	Tetromino* activeTetromino;
	std::vector<Vector2> squares;
	Color defaultSquaresColor = DARKGRAY;
	unsigned int playerScore = 0;

	ShapeType GenerateNextTetromino();

	Tetromino* CreateTetromino();
	void OnTetrominoTouchesGround();

public:
	Game();

	void StartGame();
	bool GetGameState();
	void EndGame();

	inline unsigned int GetPlayerScore() const {
		return playerScore;
	}

	inline unsigned int FindValueInList(std::vector<Vector2>& list, Vector2 value) {
		for (int i = 0; i < list.size(); ++i) {
			Vector2 vec = list[i];
			if (vec.x == value.x && vec.y == value.y) {
				return i;
			}
		}

		return -1;
	}

	inline void ClearCompletedLineIfThereAny() {
		std::map<float, std::set<float>> yToXValues;

		for (int i = 0; i < squares.size(); ++i) {
			yToXValues[squares[i].y].insert(squares[i].x);
		}

		int maxCount = INT_MIN;
		float maxYValue = -1;
		for (int i = 0; i < squares.size(); ++i) {
			int curSize = yToXValues[squares[i].y].size();
			if (curSize >= maxCount) {
				maxCount = curSize;
				maxYValue = squares[i].y;
			}
		}

		if (maxCount == Constants::blocksPerLine && maxYValue != -1) {
			// clear the completed line
			Vector2 deletedVector = { -1, -1 };
			for (int i = 0; i < yToXValues[maxYValue].size(); ++i) {

				std::vector<float> curXValues(yToXValues[maxYValue].begin(), yToXValues[maxYValue].end());

				for (int j = 0; j < curXValues.size(); ++j) {
					Vector2 valueToSearchFor = { curXValues[j], maxYValue };

					unsigned int foundVecIndex = FindValueInList(squares, valueToSearchFor);

					if (foundVecIndex != -1) {
						// remove the square from the list of squares
						deletedVector = squares[foundVecIndex];
						squares.erase(squares.begin() + foundVecIndex);
					}
				}
			}

			if (deletedVector.x != -1 && deletedVector.y != -1) {
				for (Vector2& vec : squares) {
					if (vec.y < deletedVector.y) {
						vec.y += Constants::blockWidthInPixels;
					}
				}
			}
		}
	}

	Color GetColorBasedOnShapeType(ShapeType shapeType);

	void DrawActiveTet();
	void DrawSquares();
	void RotateActiveTet();
	void MoveActiveTet(BlocksMoveDirection dir);

	~Game();
};