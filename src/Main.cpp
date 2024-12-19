

#include "../include/MenuScene.h"
#include "../include/GameScene.h"
#include "../include/SceneManager.h"

#include "../include/Globals.h"

#include "../include/UI/TextDisplay.h"

void* globalShader = 0;
bool shouldWindowClose = false;

int main() {
	InitWindow(Constants::screenWidth, Constants::screenHeight, "Falling Blocks");
	SetTargetFPS(60);

	Font font = LoadFont("resources/good timing bd.otf");

	Vector2 myNameSize = MeasureTextEx(font, "Ahmed Magdy", 24.0f, 0.0f);
	TextDisplayUI loadingText("Ahmed Magdy", { (Constants::screenWidth - myNameSize.x) / 2, (Constants::screenHeight - myNameSize.y) / 2 }, WHITE, font, Constants::normalTextFontSize);

	BeginDrawing();
	ClearBackground(BLACK);
	loadingText.Draw();
	EndDrawing();

	Shader shader = LoadShader(0, "resources/fragment.shader");
	globalShader = &shader;
	float resolution[] = { Constants::screenWidth, Constants::screenHeight };
	SetShaderValue(shader, GetShaderLocation(shader, "resolution"), resolution, SHADER_UNIFORM_VEC2);

	Texture2D bloomTexture = LoadTexture("resources/bloom.png");
	SetShaderValueTexture(shader, GetShaderLocation(shader, "bloomTexture"), bloomTexture);

	InitAudioDevice();

	SceneManager sceneManager;

	MenuScene menuScene(&sceneManager);
	menuScene.Init();

	GameScene gameScene(&sceneManager);
	gameScene.Init();

	sceneManager.AddScene("MenuScene", &menuScene);
	sceneManager.AddScene("GameScene", &gameScene);

	Sound bgMusic = LoadSound("resources/bg-music.ogg");
	SetSoundVolume(bgMusic, 0.3f);
	PlaySound(bgMusic);

	SetExitKey(0);
	int timeUniformLocation = GetShaderLocation(shader, "time");

	while (!WindowShouldClose()) {
		if (shouldWindowClose) {
			break;
		}
		float time = static_cast<float>(GetTime());
		SetShaderValue(shader, timeUniformLocation, &time, SHADER_UNIFORM_FLOAT);

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

	UnloadTexture(bloomTexture);

	CloseAudioDevice();

	CloseWindow();
}