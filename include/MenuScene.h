#pragma once

extern "C" {
#include <raylib.h>
}

#include "Scene.h"
#include "SceneManager.h"

#include "Tetrominos.h"

#include "UI/TextDisplay.h"
#include "UI/ButtonUI.h"

class MenuScene : public Scene {
private:
	SceneManager* manager;
	Font font;
	TextDisplayUI* titleText = nullptr;
	ButtonUI* startGameBtn = nullptr, *exitGameBtn = nullptr;

	std::vector<Tetromino> tets;

public:
	MenuScene(SceneManager *manager);

	void Init();
	void Update();
	void Render();

	~MenuScene();
};