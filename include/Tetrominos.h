#pragma once

struct Constants {
	static const int blocksWidth = 10;
	static const int blocksHeight = 20;
	static const int blockWidthInPixels = 35;
	static const int roationAmount = 90;
	static const int width = 1050;
	static const int height = 595;
	static const int tetInitialPosition = 35 * 2;
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

enum BlockColors : unsigned char {
	Black = 0x0,
	Green = 0x1,
	Blue = 0x2,
	DarkGray = 0x3,
	Yellow = 0x4,
	Cyan = 0x5,
	Red = 0x6,
	Pink = 0x7,
	Brown = 0x8
};

enum BlocksMoveDirection : unsigned char {
	MoveLeft,
	MoveRight,
	MoveDown
};

class Tetrominos {
private:
	static const int squaresCount = 4;

	int lastRotationAngle = 0;

	double ConvertDegreesToRadians(int deg);

	float GetMinXValue();

	float GetMaxXValue();

	Vector2 CalculateCenteroidPoint();

public:
	unsigned char color;
	ShapeType shapeType;
	Vector2 size;
	Vector2 squares[squaresCount];

	Tetrominos(ShapeType shapeType, unsigned char color, float initX, float initY);

	void Draw();

	void Move(float speed, BlocksMoveDirection dir);

	void Rotate();
};