extern "C" {
	#include <raylib.h>
	#include <raymath.h>
}

#include "../include/Game.h"
#include "../include/Tetrominos.h"

#include <vector>
#include <algorithm>
#include <iostream>

Game::Game() 
	: activeTetromino(nullptr), isPlaying(false) {
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
	
	if ((i - 1) < shapesBag.size()) {
		ShapeType selectedShape = shapesBag[i];
		i = 0;
		return selectedShape;
	}

	return shapesBag[i++];
}

Color Game::GetColorBasedOnShapeType(ShapeType shapeType) {
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

Tetromino* Game::CreateTetromino() {
	ShapeType generatedShape = GenerateNextTetromino();
	return new Tetromino(generatedShape, GetColorBasedOnShapeType(generatedShape), Constants::tetInitialPositionX, Constants::tetInitialPositionY);
}

void Game::StartGame() {
	isPlaying = true;
	activeTetromino = CreateTetromino();
}

bool Game::GetGameState() {
	return isPlaying;
}

void Game::EndGame() {
	isPlaying = false;
}

void Game::RotateActiveTet() {
	if (activeTetromino != nullptr) {
		activeTetromino->Rotate(squares);
	}
}

void Game::OnTetrominoTouchesGround() {
	activeTetromino = CreateTetromino();

	// TODO: check for complete lines
	ClearCompletedLineIfThereAny();

	// TODO: increament the player's score
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

void Game::DrawSquares() {
	for (Vector2& sq : squares) {
		DrawRectangle((int)sq.x, (int)sq.y, Constants::blockWidthInPixels, Constants::blockWidthInPixels, defaultSquaresColor);
		DrawRectangleLines((int)sq.x, (int)sq.y, Constants::blockWidthInPixels, Constants::blockWidthInPixels, BLACK);
	}
}

Game::~Game() {

}