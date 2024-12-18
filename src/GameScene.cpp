#include <raylib.h>


#include "../include/GameScene.h"
#include "../include/Tetrominos.h"

#include "../include/SceneManager.h"

#include "../include/Globals.h"

#include "../include//UI/TextDisplay.h"

GameScene::GameScene(SceneManager* manager) : manager(manager) {
	font = LoadFont("resources/good timing bd.otf");
}

void GameScene::Init() {
	game.StartGame();

	Vector2 gameOverSize = MeasureTextEx(font, "Game Over", 50.0f, 0.0f);
	gameOverText = new TextDisplayUI("Game Over", { (Constants::screenWidth - gameOverSize.x) / 2, (Constants::screenHeight - gameOverSize.y) / 2 }, WHITE, font, 50.0f);
	gameOverText->EnableHasOverlayBackground({ 0.0f, 0.0f }, { Constants::screenWidth, Constants::screenHeight }, Color{ BLACK.r, BLACK.g, BLACK.b, 150 });

	Vector2 pauseSize = MeasureTextEx(font, "Pause", 50.0f, 0.0f);
	pauseText = new TextDisplayUI("Pause", { (Constants::screenWidth - pauseSize.x) / 2, (Constants::screenHeight - pauseSize.y) / 2 }, WHITE, font, 50.0f);
	pauseText->EnableHasOverlayBackground({ 0.0f, 0.0f }, { Constants::screenWidth, Constants::screenHeight }, Color{ BLACK.r, BLACK.g, BLACK.b, 150 });

	Vector2 scoreTextSize = MeasureTextEx(font, "Score: 0", Constants::normalTextFontSize, 0.0f);
	scoreText = new TextDisplayUI("Score: 0", { (Constants::screenWidth - scoreTextSize.x + Constants::boardWidth) / 2, 20 }, WHITE, font, Constants::normalTextFontSize);
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

	Shader* shader = (Shader*)globalShader;
	BeginShaderMode(*shader);
	float brodersColor[] = { RAYWHITE.r, RAYWHITE.g, RAYWHITE.b, 1.0 };
	SetShaderValue(*shader, GetShaderLocation(*shader, "color"), brodersColor, SHADER_UNIFORM_VEC4);
	DrawRectangleLines(0, 0, Constants::boardWidth, Constants::boardHeight, RAYWHITE);
	EndShaderMode();

	const char* scoreText = TextFormat("Score: %i", game.GetPlayerScore());
	this->scoreText->SetText(scoreText);
	this->scoreText->Draw();

	// overlays goes here
	switch (game.GetGameState()) {
		case GameState::GameOver: {
			gameOverText->Draw();
			if (IsKeyPressed(KEY_ENTER)) {
				game.ResetGame();
			}
			break;
		}	
		case GameState::Pause: {
			pauseText->Draw();
			if (GetKeyPressed() == KEY_ESCAPE) {
				game.SetGameState(GameState::Playing);
			}
			break;
		}
	}
}

GameScene::~GameScene() {
	UnloadFont(font);

	delete gameOverText;
	delete pauseText;
	delete scoreText;
}