#include <raylib.h>


#include "../include/GameScene.h"
#include "../include/Tetrominos.h"

void GameScene::Init() {
	game.StartGame();
	font = LoadFont("resources/good timing bd.otf");
}

void GameScene::ShowOveralyText(std::string str) const {
	DrawRectangle(0, 0, Constants::screenWidth, Constants::screenHeight, Color{ 0, 0, 0, 150 });

	const char* gameOverTxt = str.c_str();
	Vector2 gameOverSize = MeasureTextEx(font, gameOverTxt, 50.0f, 0.0f);
	DrawTextEx(font, gameOverTxt, { (Constants::screenWidth - gameOverSize.x) / 2, (Constants::screenHeight - gameOverSize.y) / 2 }, gameOverSize.y, 0.0f, RAYWHITE);
}

void GameScene::Update() {
	switch (GetKeyPressed()) {
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
		fallInterval = 0.1f;
	}
	if (IsKeyUp(KEY_S)) {
		fallInterval = 1.0f;
	}

	fallTimer += GetFrameTime();

	if (fallTimer >= fallInterval) {
		game.MoveActiveTet(BlocksMoveDirection::MoveDown);
		fallTimer = 0.0f;
	}
}

void GameScene::Render() {
	if (game.GetGameState() == GameState::Playing) {
		Update();

		if (IsKeyPressed(KEY_ESCAPE)) {
			game.SetGameState(GameState::Pause);
		}
	}

	game.DrawActiveTet();

	game.DrawSquares();

	game.DrawNextTet();

	DrawRectangleLines(0, 0, Constants::boardWidth, Constants::boardHeight, RAYWHITE);

	const char* scoreText = TextFormat("Score: %i", game.GetPlayerScore());
	Vector2 scoreTextSize = MeasureTextEx(font, scoreText, Constants::textFontSize, 0.0f);
	DrawTextEx(font, scoreText, { (Constants::screenWidth - scoreTextSize.x + Constants::boardWidth) / 2, 20 }, Constants::textFontSize, 0.0f, RAYWHITE);

	// overlays goes here
	switch (game.GetGameState()) {
		case GameState::GameOver: {
			ShowOveralyText("Game Over");
			if (IsKeyPressed(KEY_ENTER)) {
				game.ResetGame();
			}
			break;
		}	
		case GameState::Pause: {
			ShowOveralyText("Pause");
			if (GetKeyPressed() == KEY_ESCAPE) {
				game.SetGameState(GameState::Playing);
			}
			break;
		}
	}
}

GameScene::~GameScene() {
	UnloadFont(font);
}