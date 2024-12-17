

#include "../include/GameScene.h"
#include "../include/SceneManager.h"

#include "../include/Globals.h"

void* globalShader = 0;

int main() {
	InitWindow(Constants::screenWidth, Constants::screenHeight, "Falling Blocks");
	SetTargetFPS(60);

	Font font = LoadFont("resources/good timing bd.otf");

	Shader shader = LoadShader(0, "resources/fragment.shader");
	globalShader = &shader;
	float resolution[] = { Constants::screenWidth, Constants::screenHeight };
	SetShaderValue(shader, GetShaderLocation(shader, "resolution"), resolution, SHADER_UNIFORM_VEC2);

	BeginDrawing();
	ClearBackground(BLACK);
	const char* myName = "Loading...";
	Vector2 myNameSize = MeasureTextEx(font, myName, 24.0f, 0.0f);
	DrawTextEx(font, myName, { (Constants::screenWidth - myNameSize.x) / 2, (Constants::screenHeight - myNameSize.y)/2 }, 24.0f, 0.0f, RAYWHITE);
	EndDrawing();

	InitAudioDevice();

	SceneManager sceneManager;

	GameScene gameScene;
	gameScene.Init();

	sceneManager.AddScene("GameScene", &gameScene);

	Sound bgMusic = LoadSound("resources/bg-music.ogg");
	SetSoundVolume(bgMusic, 0.3f);
	PlaySound(bgMusic);

	SetExitKey(0);

	while (!WindowShouldClose()) {
		float time = static_cast<float>(GetTime());
		SetShaderValue(shader, GetShaderLocation(shader, "time"), &time, SHADER_UNIFORM_FLOAT);

		if (!IsSoundPlaying(bgMusic)) {
			PlaySound(bgMusic);
		}

		BeginDrawing();
		ClearBackground(BLACK);
		sceneManager.RenderCurrentScene();
		EndDrawing();
	}

	UnloadFont(font);

	UnloadShader(shader);

	UnloadSound(bgMusic);

	CloseAudioDevice();

	CloseWindow();
}