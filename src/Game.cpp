extern "C" {
	#include <raylib.h>
	#include <raymath.h>
}

#include "../include/Game.h"
#include "../include/Tetrominos.h"

#include <vector>
#include <algorithm>

Game::Game() 
	: activeTetrominos(nullptr), isPlaying(false) {
}

ShapeType Game::GenerateNextTetrominos() {
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

void Game::StartGame() {
	isPlaying = true;
	ShapeType generatedShape = GenerateNextTetrominos();
	activeTetrominos = new Tetrominos(generatedShape, 0x1, Constants::tetInitialPosition, Constants::tetInitialPosition);
}

bool Game::GetGameState() {
	return isPlaying;
}

void Game::EndGame() {
	isPlaying = false;
}

void Game::RotateActiveTet() {
	if (activeTetrominos != nullptr) {
		activeTetrominos->Rotate();
	}
}

void Game::MoveActiveTet(BlocksMoveDirection dir) {
	if (activeTetrominos != nullptr) {
		activeTetrominos->Move(1.0f, dir);
	}
}

void Game::DrawActiveTet() {
	if (activeTetrominos != nullptr) {
		activeTetrominos->Draw();
	}
}

Game::~Game() {

}