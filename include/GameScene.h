#pragma once

#include "Scene.h"
#include "Game.h"

#include "SceneManager.h"

#include "UI/TextDisplay.h"

class GameScene : public Scene {
private:
	Game game;
	float fallTimer = 0.0f;
	float fallInterval = 1.0f;
	Font font;
	int colorUniformLocation = -1;
	SceneManager* manager;

	TextDisplayUI* gameOverText = nullptr, * pauseText = nullptr, * scoreText = nullptr, * curLevelText = nullptr;

public:
	GameScene(SceneManager* manager);

	void Init();
	void Update();
	void Render();

	~GameScene();
};