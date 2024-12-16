extern "C" {
#include <raylib.h>
}

#include "../include/Tetrominos.h"
#include "../include/Game.h"

int main() {
	InitWindow(Constants::screenWidth, Constants::screenHeight, "Falling Blocks");
	SetTargetFPS(60);

	Game game;
	game.StartGame();

	float fallTimer = 0.0f;
	float fallInterval = 1.0f;

	while (!WindowShouldClose()) {
		ClearBackground(BLACK);

		switch(GetKeyPressed()) {
		case KEY_A:
			game.MoveActiveTet(BlocksMoveDirection::MoveLeft);
			break;
		case KEY_D:
			game.MoveActiveTet(BlocksMoveDirection::MoveRight);
			break;
		case KEY_W:
			game.RotateActiveTet();
			break;
		}

		if (IsKeyDown(KEY_S)) {
			fallInterval = 0.25f;
		}
		if (IsKeyUp(KEY_S)) {
			fallInterval = 1.0f;
		}

		fallTimer += GetFrameTime();

		if (fallTimer >= fallInterval) {
			game.MoveActiveTet(BlocksMoveDirection::MoveDown);
			fallTimer = 0.0f;
		}

		BeginDrawing();

		game.DrawActiveTet();

		game.DrawSquares();

		DrawRectangleLines(0, 0, Constants::boardWidth, Constants::boardHeight, RAYWHITE);

		const char* scoreText = TextFormat("Score: %i", game.GetPlayerScore());
		Vector2 scoreTextSize = MeasureTextEx(GetFontDefault(), scoreText, Constants::textFontSize, 0.0f);
		DrawText(scoreText, (Constants::screenWidth - scoreTextSize.x) / 2, 20, Constants::textFontSize, RAYWHITE);

		EndDrawing();
	}

	CloseWindow();
}