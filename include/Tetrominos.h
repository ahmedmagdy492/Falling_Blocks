#pragma once

#include <vector>
#include <raylib.h>

struct Constants {
	static const int screenWidth = 1080;
	static const int screenHeight = 920;
	static const int normalTextFontSize = 24;

	static const int blocksWidth = 10;
	static const int blocksHeight = 20;
	static const int blockWidthInPixels = 35;
	static const int roationAmount = 90;
	static const int boardWidth = 455;  // in pixels
	static const int boardHeight = 910;  // in pixels
	static const int tetInitialPositionX = 175;
	static const int tetInitialPositionY = 0;
	static const int blocksPerCompleteLine = boardWidth / blockWidthInPixels;

	// Score Calcuations
	static const int singleScoreValue = 40;
	static const int doubleScoreValue = 100;
	static const int tripleScoreValue = 300;
	static const int tetrisScoreValue = 1200;
};

enum ShapeType {
	LShape,
	JShape,
	TShape,
	OShape,
	IShape,
	SShape,
	ZShape
};

enum BlocksMoveDirection : unsigned char {
	MoveLeft,
	MoveRight,
	MoveDown
};

class Tetromino {
private:
	static const int squaresCount = 4;

	double ConvertDegreesToRadians(int deg);
	float GetMinXValue();
	float GetMaxXValue();
	float GetMaxYValue();
	float GetMinYValue();
	bool IsSquareOverlapWithAnyOtherSquare(Vector2 squarePos, std::vector<Vector2>& existingSquares);

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
	Color color;
	ShapeType shapeType;
	Vector2 size;
	Vector2 squares[squaresCount];
	Vector2* centerPiece = nullptr;

	Tetromino(ShapeType shapeType, Color color, float initX, float initY);

	void Draw();
	bool Move(float speed, BlocksMoveDirection dir, std::vector<Vector2>& existingSquares);
	void Rotate(std::vector<Vector2>& existingSquares);
	static void CalculateSquareValuesBasedOnShapeType(Vector2** centerPiece, Vector2 squares[], ShapeType shapeType, float initX, float initY);
};