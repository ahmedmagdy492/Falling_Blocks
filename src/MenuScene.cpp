
#include "../include/MenuScene.h"

#include "../include/Tetrominos.h"

#include <vector>

MenuScene::MenuScene(SceneManager* manager) : manager(manager) {
	font = LoadFont("resources/good timing bd.otf");
}

void MenuScene::Init() {
	Vector2 size = MeasureTextEx(font, "Falling Blocks", 80.0f, 0.0f);
	titleText = new TextDisplayUI("Falling Blocks", { (Constants::screenWidth - size.x) / 2, 50.0f }, WHITE, font, 80.0f);

	for (int i = 0; i < 20; ++i) {
		Vector2 randPos = { GetRandomValue(0, Constants::screenWidth), GetRandomValue(0, Constants::screenHeight) };
		ShapeType randShapeType = static_cast<ShapeType>(GetRandomValue(static_cast<int>(ShapeType::LShape), static_cast<int>(ShapeType::ZShape)));
		unsigned char randColorComp = static_cast<unsigned char>(GetRandomValue(100, 200));
		Color randColor = Color{ randColorComp, randColorComp, randColorComp, randColorComp };
		Tetromino tet(randShapeType, randColor, randPos.x, randPos.y);
		
		tets.push_back(tet);
	}
}

void MenuScene::Update() {
	switch (GetKeyPressed()) {
	
	}
}

void MenuScene::Render() {

	for (Tetromino& tet : tets) {
		tet.Draw();
	}

	// text rendering
	titleText->Draw();
}

MenuScene::~MenuScene() {
	UnloadFont(font);

	delete titleText;
}