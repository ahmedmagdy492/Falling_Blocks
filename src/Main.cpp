extern "C" {
#include <raylib.h>
}

#include "../include/GameScene.h"
#include "../include/SceneManager.h"

int main() {
	InitWindow(Constants::screenWidth, Constants::screenHeight, "Falling Blocks");
	SetTargetFPS(60);

	Font font = LoadFont("resources/good timing bd.otf");

	BeginDrawing();
	ClearBackground(Color{ 26, 35, 126, 1 });
	const char* myName = "Loading...";
	Vector2 myNameSize = MeasureTextEx(font, myName, 24.0f, 0.0f);
	DrawTextEx(font, myName, { (Constants::screenWidth - myNameSize.x) / 2, (Constants::screenHeight - myNameSize.y)/2 }, 24.0f, 0.0f, RAYWHITE);
	EndDrawing();

	InitAudioDevice();

	GameScene gameScene;
	gameScene.Init();
	SceneManager sceneManager;

	sceneManager.AddScene("GameScene", &gameScene);

	Sound bgMusic = LoadSound("resources/bg-music.ogg");
	SetSoundVolume(bgMusic, 0.3);
	PlaySound(bgMusic);

	SetExitKey(0);

	while (!WindowShouldClose()) {

		if (!IsSoundPlaying(bgMusic)) {
			PlaySound(bgMusic);
		}

		ClearBackground(Color{ 26, 35, 126, 1 });

		BeginDrawing();
		sceneManager.RenderCurrentScene();
		EndDrawing();
	}

	UnloadFont(font);

	UnloadSound(bgMusic);

	CloseAudioDevice();

	CloseWindow();
}