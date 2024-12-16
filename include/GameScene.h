#pragma once

#include "Scene.h"
#include "Game.h"

class GameScene : public Scene {
private:
	Game game;
	float fallTimer = 0.0f;
	float fallInterval = 1.0f;
	Font font;

public:
	void Init();
	void Update();
	void Render();

	~GameScene();
};