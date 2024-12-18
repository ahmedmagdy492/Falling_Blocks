#include "../include/Game.h"
#include "../include/Tetrominos.h"

#include "../include/Globals.h"

#include <vector>
#include <algorithm>
#include <iostream>

Game::Game() 
	: activeTetromino(nullptr), nextTetromino(ShapeType::OShape) {
	sounds[0] = LoadSound("resources/line-clear.ogg");
	sounds[1] = LoadSound("resources/game-over.ogg");
	sounds[2] = LoadSound("resources/piece-down.ogg");
}

ShapeType Game::GenerateNextTetromino() {
	static std::vector<ShapeType> shapesBag;
	shapesBag.push_back(ShapeType::TShape);
	shapesBag.push_back(ShapeType::JShape);
	shapesBag.push_back(ShapeType::LShape);
	shapesBag.push_back(ShapeType::OShape);
	shapesBag.push_back(ShapeType::SShape);
	shapesBag.push_back(ShapeType::ZShape);
	shapesBag.push_back(ShapeType::IShape);

	static int i = 0;

	if (i == 0) {
		std::random_shuffle(shapesBag.begin(), shapesBag.end());
		return shapesBag[i++];
	}
	
	if (i < shapesBag.size()) {
		ShapeType selectedShape = shapesBag[i];
		i = 0;
		return selectedShape;
	}

	return shapesBag[i++];
}

Tetromino* Game::CreateTetromino(ShapeType shapeType) {
	return new Tetromino(shapeType, GetColorBasedOnShapeType(shapeType), Constants::tetInitialPositionX, Constants::tetInitialPositionY);
}

void Game::StartGame() {
	curGameState = GameState::Playing;
	activeTetromino = CreateTetromino(GenerateNextTetromino());
	nextTetromino = GenerateNextTetromino();
}

void Game::EndGame() {
	curGameState = GameState::GameOver;
}

void Game::RotateActiveTet() {
	if (activeTetromino != nullptr) {
		activeTetromino->Rotate(squares);
	}
}

void Game::OnTetrominoTouchesGround() {
	activeTetromino = CreateTetromino(nextTetromino);
	nextTetromino = GenerateNextTetromino();
	ClearCompletedLineIfThereAny();

	for (Vector2& vec : squares) {
		if ((vec.y - Constants::blockWidthInPixels) <= 0) {
			SetGameState(GameState::GameOver);
			PlaySound(sounds[1]);
			break;
		}
	}
}

void Game::MoveActiveTet(BlocksMoveDirection dir) {
	if (activeTetromino != nullptr) {
		bool wasMoveSuccessful = activeTetromino->Move(1.0f, dir, squares);
		if (dir == BlocksMoveDirection::MoveDown && !wasMoveSuccessful) {
			for (const Vector2& sq : activeTetromino->squares) {
				squares.push_back(sq);
			}

			delete activeTetromino;
			activeTetromino = nullptr;
			OnTetrominoTouchesGround();
		}
	}
}

void Game::DrawActiveTet() {
	if (activeTetromino != nullptr) {
		activeTetromino->Draw();
	}
}

unsigned int Game::GetTetWidth(ShapeType type) {
	switch (type) {
	case ShapeType::OShape:
	case ShapeType::JShape:
	case ShapeType::LShape:
		return 2 * Constants::blockWidthInPixels;
	case ShapeType::IShape:
		return Constants::blockWidthInPixels;
	case ShapeType::SShape:
	case ShapeType::TShape:
		return 3 * Constants::blockWidthInPixels;
	default:
		return 3 * Constants::blockWidthInPixels;
	}
}

void Game::DrawNextTet() {
	Vector2 squares[4];
	Vector2 centerPiece;
	Vector2* pCenterPiece;
	Tetromino::CalculateSquareValuesBasedOnShapeType(&pCenterPiece, squares, nextTetromino, (Constants::screenWidth - GetTetWidth(nextTetromino) + Constants::boardWidth) / 2, 120);

	for (Vector2& sq : squares) {
		DrawRectangle((int)sq.x, (int)sq.y, Constants::blockWidthInPixels, Constants::blockWidthInPixels, GetColorBasedOnShapeType(nextTetromino));
		DrawRectangleLines((int)sq.x, (int)sq.y, Constants::blockWidthInPixels, Constants::blockWidthInPixels, BLACK);
	}
}

void Game::DrawSquares() {
	Shader* shader = (Shader*)globalShader;
	float color[] = { defaultSquaresColor.r, defaultSquaresColor.g, defaultSquaresColor.b, 1.0f };
	SetShaderValue(*shader, GetShaderLocation(*shader, "color"), color, SHADER_UNIFORM_VEC4);

	BeginShaderMode(*shader);
	for (Vector2& sq : squares) {
		DrawRectangle((int)sq.x, (int)sq.y, Constants::blockWidthInPixels, Constants::blockWidthInPixels, BLACK);
	}
	EndShaderMode();

	for (Vector2& sq : squares) {
		DrawRectangleLines((int)sq.x, (int)sq.y, Constants::blockWidthInPixels, Constants::blockWidthInPixels, defaultSquaresColor);
	}
}

void Game::ClearCompeleteLine(std::set<float>& xValues, float maxYValue) {
	Vector2 deletedVector = { -1, -1 };

	std::vector<float> curXValues(xValues.begin(), xValues.end());

	for (int j = 0; j < curXValues.size(); ++j) {
		Vector2 valueToSearchFor = { curXValues[j], maxYValue };

		unsigned int foundVecIndex = FindValueInList(squares, valueToSearchFor);

		if (foundVecIndex != -1) {
			// remove the square from the list of squares
			deletedVector = squares[foundVecIndex];
			squaresToRemove.push_back(std::pair<float, float>(valueToSearchFor.x, valueToSearchFor.y));
		}
	}
}

void Game::ResetGame() {
	curGameState = GameState::Playing;
	if (activeTetromino != nullptr) {
		delete activeTetromino;
		activeTetromino = nullptr;
	}

	squares.clear();
	squaresToRemove.clear();
	playerScore = 0;

	StartGame();
}

void Game::ClearCompletedLineIfThereAny() {
	std::map<float, std::set<float>> yToXValues;

	for (int i = 0; i < squares.size(); ++i) {
		yToXValues[squares[i].y].insert(squares[i].x);
	}

	unsigned char noClearedLines = 0;
	std::vector<float> clearedYValues;
	for (auto& keyValue : yToXValues) {
		int curSize = keyValue.second.size();
		if (curSize == Constants::blocksPerCompleteLine) {
			clearedYValues.push_back(keyValue.first);
			std::set<float> setOfXValues = keyValue.second;
			ClearCompeleteLine(setOfXValues, keyValue.first);
			noClearedLines++;
		}
	}

	// remove cleared squares
	std::vector<Vector2> temp;
	for (Vector2& vec : squares) {
		temp.push_back(vec);
	}
	squares.clear();

	for (Vector2& vec : temp) {
		if (std::find(squaresToRemove.begin(), squaresToRemove.end(), std::pair<float, float>(vec.x, vec.y)) == squaresToRemove.end()) {
			squares.push_back(vec);
		}
	}

	temp.clear();
	squaresToRemove.clear();

	PlaySound(sounds[2]);

	// shift down lines above cleared lines
	if (noClearedLines > 0) {
		float minClearedYValue = *std::min(clearedYValues.begin(), clearedYValues.end());
		for (Vector2& vec : squares) {
			if (vec.y < minClearedYValue) {
				vec.y += Constants::blockWidthInPixels * noClearedLines;
			}
		}

		IncreamentScore(noClearedLines);
		PlaySound(sounds[0]);
	}
}

Game::~Game() {
	if (activeTetromino != nullptr) {
		delete activeTetromino;
	}

	for (int i = 0; i < soundsCount; ++i) {
		UnloadSound(sounds[i]);
	}
}