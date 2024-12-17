extern "C" {
	#include <raylib.h>
	#include <raymath.h>
}

#include "../include/Tetrominos.h"
#include "../include/Globals.h"

#include <iostream>
#include <vector>
#include <algorithm>

double Tetromino::ConvertDegreesToRadians(int deg)
{
	return deg * (PI / 180.0f);
}

float Tetromino::GetMinXValue() {
	float minXValue = squares[0].x;
	for (int i = 0; i < squaresCount; ++i) {
		if (squares[i].x < minXValue) {
			minXValue = squares[i].x;
		}
	}

	return minXValue;
}

float Tetromino::GetMaxXValue() {
	float maxXValue = squares[0].x;
	for (int i = 0; i < squaresCount; ++i) {
		if (squares[i].x >= maxXValue) {
			maxXValue = squares[i].x;
		}
	}

	return maxXValue;
}

float Tetromino::GetMinYValue() {
	float minYValue = squares[0].y;
	for (int i = 0; i < squaresCount; ++i) {
		if (squares[i].y < minYValue) {
			minYValue = squares[i].y;
		}
	}

	return minYValue;
}

float Tetromino::GetMaxYValue() {
	float maxYValue = squares[0].y;
	for (int i = 0; i < squaresCount; ++i) {
		if (squares[i].y >= maxYValue) {
			maxYValue = squares[i].y;
		}
	}

	return maxYValue;
}

Tetromino::Tetromino(ShapeType shapeType, Color color, float initX, float initY) :
	color(color), shapeType(shapeType) {
	size = { Constants::blockWidthInPixels, Constants::blockWidthInPixels };
	CalculateSquareValuesBasedOnShapeType(&centerPiece, squares, shapeType, initX, initY);
}

void Tetromino::CalculateSquareValuesBasedOnShapeType(Vector2** centerPiece, Vector2 squares[], ShapeType shapeType, float initX, float initY) {
	switch (shapeType) {
	case ShapeType::ZShape:
		squares[0] = { initX, initY };
		initX += Constants::blockWidthInPixels;
		squares[1] = { initX, initY };
		initY += Constants::blockWidthInPixels;
		squares[2] = { initX, initY };
		*centerPiece = &squares[1];
		initX += Constants::blockWidthInPixels;
		squares[3] = { initX, initY };
		break;
	case ShapeType::SShape:
		initY += Constants::blockWidthInPixels;
		squares[0] = { initX, initY };
		initX += Constants::blockWidthInPixels;
		squares[1] = { initX, initY };
		*centerPiece = &squares[1];
		initY -= Constants::blockWidthInPixels;
		squares[2] = { initX, initY };
		initX += Constants::blockWidthInPixels;
		squares[3] = { initX, initY };
		break;
	case ShapeType::LShape:
		squares[0] = { initX, initY };
		initY += Constants::blockWidthInPixels;
		squares[1] = { initX, initY };
		*centerPiece = &squares[1];
		initY += Constants::blockWidthInPixels;
		squares[2] = { initX, initY };
		initX += Constants::blockWidthInPixels;
		squares[3] = { initX, initY };
		break;
	case ShapeType::JShape:
		squares[0] = { initX, initY };
		initY += Constants::blockWidthInPixels;
		squares[1] = { initX, initY };
		*centerPiece = &squares[1];
		initY += Constants::blockWidthInPixels;
		squares[2] = { initX, initY };
		initX -= Constants::blockWidthInPixels;
		squares[3] = { initX, initY };
		break;
	case ShapeType::OShape:
		squares[0] = { initX, initY };
		initX += Constants::blockWidthInPixels;
		squares[1] = { initX, initY };
		initY += Constants::blockWidthInPixels;
		squares[2] = { initX, initY };
		initX -= Constants::blockWidthInPixels;
		squares[3] = { initX, initY };
		break;
	case ShapeType::IShape:
		squares[0] = { initX, initY };
		initY += Constants::blockWidthInPixels;
		squares[1] = { initX, initY };
		*centerPiece = &squares[1];
		initY += Constants::blockWidthInPixels;
		squares[2] = { initX, initY };
		initY += Constants::blockWidthInPixels;
		squares[3] = { initX, initY };
		break;
	case ShapeType::TShape:
		squares[0] = { initX, initY };
		initX += Constants::blockWidthInPixels;
		squares[1] = { initX, initY };
		*centerPiece = &squares[1];
		initX += Constants::blockWidthInPixels;
		squares[2] = { initX, initY };
		initX -= Constants::blockWidthInPixels;
		initY += Constants::blockWidthInPixels;
		squares[3] = { initX, initY };
		break;
	}
}

void Tetromino::Draw() {
	Shader* shader = (Shader*)globalShader;
	Color raylibColor = GetColorBasedOnShapeType(shapeType);
	float color[] = { raylibColor.r, raylibColor.g, raylibColor.b, 1.0f };
	SetShaderValue(*shader, GetShaderLocation(*shader, "color"), color, SHADER_UNIFORM_VEC4);
	BeginShaderMode(*shader);
	for (int i = 0; i < squaresCount; ++i) {
		DrawRectangleV(squares[i], size, raylibColor);
		//DrawRectangleLines(squares[i].x, squares[i].y, Constants::blockWidthInPixels, Constants::blockWidthInPixels, BLACK);
	}
	EndShaderMode();
}

bool Tetromino::IsSquareOverlapWithAnyOtherSquare(Vector2 squarePos, std::vector<Vector2>& existingSquares) {
	for (const Vector2& sq : existingSquares) {
		Rectangle boundTargetSquare = { squarePos.x, squarePos.y, Constants::blockWidthInPixels, Constants::blockWidthInPixels };
		Rectangle boundCurSquare = { sq.x, sq.y, Constants::blockWidthInPixels, Constants::blockWidthInPixels };

		if (CheckCollisionRecs(boundTargetSquare, boundCurSquare)) {
			return true;
		}
	}

	return false;
}

bool Tetromino::Move(float speed, BlocksMoveDirection dir, std::vector<Vector2>& existingSquares) {
	switch (dir) {
	case BlocksMoveDirection::MoveLeft: {
		float minXValue = GetMinXValue();

		if ((minXValue - Constants::blockWidthInPixels) >= 0) {
			for (int i = 0; i < squaresCount; ++i) {
				if (IsSquareOverlapWithAnyOtherSquare({ squares[i].x - Constants::blockWidthInPixels, squares[i].y }, existingSquares)) {
					return false;
				}
			}

			for (int i = 0; i < squaresCount; ++i) {
				squares[i].x = squares[i].x - Constants::blockWidthInPixels;
			}
		}
		else {
			return false;
		}
		break;
	}
	case BlocksMoveDirection::MoveRight: {
		float maxXValue = GetMaxXValue();

		if ((maxXValue + Constants::blockWidthInPixels) < Constants::boardWidth) {
			for (int i = 0; i < squaresCount; ++i) {
				if (IsSquareOverlapWithAnyOtherSquare({ squares[i].x + Constants::blockWidthInPixels, squares[i].y }, existingSquares)) {
					return false;
				}
			}

			for (int i = 0; i < squaresCount; ++i) {
				squares[i].x = squares[i].x + Constants::blockWidthInPixels;
			}
		}
		else {
			return false;
		}
		break;
	}
	case BlocksMoveDirection::MoveDown: {
		float maxYValue = GetMaxYValue();
		if ((maxYValue + Constants::blockWidthInPixels) < Constants::boardHeight) {
			for (int i = 0; i < squaresCount; ++i) {
				if (IsSquareOverlapWithAnyOtherSquare({ squares[i].x, squares[i].y + Constants::blockWidthInPixels }, existingSquares)) {
					return false;
				}
			}

			for (int i = 0; i < squaresCount; ++i) {
				squares[i].y = squares[i].y + Constants::blockWidthInPixels;
			}
		}
		else {
			return false;
		}
		break;
	}
	}

	return true;
}

void Tetromino::Rotate(std::vector<Vector2>& existingSquares) {
	if (shapeType == ShapeType::OShape)
		return;

	Vector2 tempSquares[4];

	for (int i = 0; i < squaresCount; ++i) {
		tempSquares[i] = { squares[i].x, squares[i].y };
	}

	for (int i = 0; i < squaresCount; ++i) {
		Matrix transMat = MatrixTranslate(-(*centerPiece).x, -(*centerPiece).y, 0.0f);
		tempSquares[i] = Vector2Transform({ tempSquares[i].x, tempSquares[i].y }, transMat);
		double rotationAngleInRad = ConvertDegreesToRadians(Constants::roationAmount);
		Matrix rotateMat = MatrixRotateZ(rotationAngleInRad);
		tempSquares[i] = Vector2Transform(tempSquares[i], rotateMat);
		Matrix transBackMat = MatrixTranslate((*centerPiece).x, (*centerPiece).y, 0.0f);
		tempSquares[i] = Vector2Transform(tempSquares[i], transBackMat);
	}

	// collision detection with other tetrominos
	for (int i = 0; i < squaresCount; ++i) {
		if (
			IsSquareOverlapWithAnyOtherSquare({ tempSquares[i].x, tempSquares[i].y }, existingSquares) 
			|| IsSquareOverlapWithAnyOtherSquare({ tempSquares[i].x + Constants::blockWidthInPixels, tempSquares[i].y }, existingSquares) 
			|| IsSquareOverlapWithAnyOtherSquare({ tempSquares[i].x, tempSquares[i].y + Constants::blockWidthInPixels }, existingSquares)
			|| IsSquareOverlapWithAnyOtherSquare({ tempSquares[i].x + Constants::blockWidthInPixels, tempSquares[i].y + Constants::blockWidthInPixels }, existingSquares)
			) {
			return;
		}
	}

	// collision detection with window boundaries
	for (int i = 0; i < squaresCount; ++i) {
		if (tempSquares[i].x > Constants::boardWidth || (tempSquares[i].x + Constants::blockWidthInPixels) > Constants::boardWidth || tempSquares[i].x < 0) {
			return;
		}
		if (tempSquares[i].y > Constants::boardHeight || (tempSquares[i].y + Constants::blockWidthInPixels) > Constants::boardHeight) {
			return;
		}
	}

	for (int i = 0; i < squaresCount; ++i) {
		squares[i].x = tempSquares[i].x;
		squares[i].y = tempSquares[i].y;
	}
}