extern "C" {
	#include <raylib.h>
	#include <raymath.h>
}

#include "../include/Tetrominos.h"
#include <iostream>
#include <vector>
#include <algorithm>

double Tetrominos::ConvertDegreesToRadians(int deg)
{
	return deg * (PI / 180.0f);
}

float Tetrominos::GetMinXValue() {
	float minXValue = INT_MAX;
	for (int i = 0; i < squaresCount; ++i) {
		if (squares[i].x < minXValue) {
			minXValue = squares[i].x;
		}
	}

	return minXValue;
}

float Tetrominos::GetMaxXValue() {
	float maxXValue = INT_MIN;
	for (int i = 0; i < squaresCount; ++i) {
		if (squares[i].x > maxXValue) {
			maxXValue = squares[i].x;
		}
	}

	return maxXValue;
}

Vector2 Tetrominos::CalculateCenteroidPoint() {
	float xSum = 0.0f, ySum = 0.0f;

	for (int i = 0; i < squaresCount; ++i) {
		xSum += squares[i].x;
		ySum += squares[i].y;
	}

	return { xSum / squaresCount, ySum / squaresCount };
}

Tetrominos::Tetrominos(ShapeType shapeType, unsigned char color, float initX, float initY) :
	color(color), shapeType(shapeType) {
	size = { Constants::blockWidthInPixels, Constants::blockWidthInPixels };
	switch (shapeType) {
	case ShapeType::ZShape:
		squares[0] = { initX, initY };
		initX += Constants::blockWidthInPixels;
		squares[1] = { initX, initY };
		initY += Constants::blockWidthInPixels;
		squares[2] = { initX, initY };
		initX += Constants::blockWidthInPixels;
		squares[3] = { initX, initY };
		break;
	case ShapeType::SShape:
		initY += Constants::blockWidthInPixels;
		squares[0] = { initX, initY };
		initX += Constants::blockWidthInPixels;
		squares[1] = { initX, initY };
		initY -= Constants::blockWidthInPixels;
		squares[2] = { initX, initY };
		initX += Constants::blockWidthInPixels;
		squares[3] = { initX, initY };
		break;
	case ShapeType::LShape:
		squares[0] = { initX, initY };
		initY += Constants::blockWidthInPixels;
		squares[1] = { initX, initY };
		initY += Constants::blockWidthInPixels;
		squares[2] = { initX, initY };
		initX += Constants::blockWidthInPixels;
		squares[3] = { initX, initY };
		break;
	case ShapeType::JShape:
		squares[0] = { initX, initY };
		initY += Constants::blockWidthInPixels;
		squares[1] = { initX, initY };
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
		initY += Constants::blockWidthInPixels;
		squares[2] = { initX, initY };
		initY += Constants::blockWidthInPixels;
		squares[3] = { initX, initY };
		break;
	case ShapeType::TShape:
		squares[0] = { initX, initY };
		initX += Constants::blockWidthInPixels;
		squares[1] = { initX, initY };
		initX += Constants::blockWidthInPixels;
		squares[2] = { initX, initY };
		initX -= Constants::blockWidthInPixels;
		initY += Constants::blockWidthInPixels;
		squares[3] = { initX, initY };
		break;
	}
}

void Tetrominos::Draw() {
	for (int i = 0; i < squaresCount; ++i) {
		DrawRectangleV(squares[i], size, RED);
		DrawRectangleLines(squares[i].x, squares[i].y, Constants::blockWidthInPixels, Constants::blockWidthInPixels, BLACK);
	}
}

void Tetrominos::Move(float speed, BlocksMoveDirection dir) {
	switch (dir) {
	case BlocksMoveDirection::MoveLeft: {
		float minXValue = GetMinXValue();

		if ((minXValue - Constants::blockWidthInPixels) >= 0) {
			for (int i = 0; i < squaresCount; ++i) {
				squares[i].x = squares[i].x - Constants::blockWidthInPixels;
			}
		}
		break;
	}
	case BlocksMoveDirection::MoveRight: {
		float maxXValue = GetMaxXValue();

		if ((maxXValue + Constants::blockWidthInPixels) < Constants::width) {
			for (int i = 0; i < squaresCount; ++i) {
				squares[i].x = squares[i].x + Constants::blockWidthInPixels;
			}
		}
		break;
	}
	}
}

void Tetrominos::Rotate() {
	if (shapeType == ShapeType::OShape)
		return;

	std::vector<float> xValues, yValues;

	for (int i = 0; i < squaresCount; ++i) {
		xValues.push_back(squares[i].x);
		yValues.push_back(squares[i].y);
	}

	std::sort(xValues.begin(), xValues.end());
	std::sort(yValues.begin(), yValues.end());

	float midX = Lerp(xValues[1], xValues[2], 0.5);
	float midY = Lerp(yValues[1], yValues[2], 0.5);

	if (lastRotationAngle < 360) {
		lastRotationAngle += Constants::roationAmount;
	}
	else {
		lastRotationAngle = 0;
	}

	for (int i = 0; i < squaresCount; ++i) {
		Matrix transMat = MatrixTranslate(-midX, -midY, 0.0f);
		squares[i] = Vector2Transform(squares[i], transMat);
		double rotationAngleInRad = ConvertDegreesToRadians(Constants::roationAmount);
		Matrix rotateMat = MatrixRotateZ(rotationAngleInRad);
		squares[i] = Vector2Transform(squares[i], rotateMat);
		Matrix transBackMat = MatrixTranslate(midX, midY, 0.0f);
		squares[i] = Vector2Transform(squares[i], transBackMat);
	}
}