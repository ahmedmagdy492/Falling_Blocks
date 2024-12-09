extern "C" {
#include <raylib.h>
}

#include "../include/Tetrominos.h"
#include "../include/Game.h"

int main() {
	InitWindow(Constants::width, Constants::height, "Tetris Demo");
	SetTargetFPS(60);

	Game game;

	game.StartGame();

	while (!WindowShouldClose()) {
		ClearBackground(BLACK);

		switch(GetKeyPressed()) {
		case KEY_A:
			game.MoveActiveTet(BlocksMoveDirection::MoveLeft);
			break;
		case KEY_D:
			game.MoveActiveTet(BlocksMoveDirection::MoveRight);
			break;
		case KEY_S:
			break;
		case KEY_W:
			game.RotateActiveTet();
			break;
		}

		BeginDrawing();

		game.DrawActiveTet();

		EndDrawing();
	}

	CloseWindow();
}