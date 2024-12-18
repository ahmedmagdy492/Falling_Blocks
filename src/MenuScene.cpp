
#include "../include/MenuScene.h"

#include "../include/Tetrominos.h"

#include "../include/Globals.h"

#include <vector>

static bool isStartGameClicked = false;

static void onStartGameButtonClick() {
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		isStartGameClicked = true;
	}
}

static void onExitGameButtonClick() {
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		shouldWindowClose = true;
	}
}

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

	startGameBtn = new ButtonUI("Start Game", font, Constants::aboveNormalTextFontSize, {0.0f, 0.0f}, onStartGameButtonClick);
	startGameBtn->CenterButton(Constants::screenWidth, Constants::screenHeight);
	exitGameBtn = new ButtonUI("Exit", font, Constants::aboveNormalTextFontSize, {0.0f, 0.0f}, onExitGameButtonClick);
	exitGameBtn->PlaceBeneath(*startGameBtn, 10.0f);
}

void MenuScene::Update() {
	if (isStartGameClicked) {
		manager->SwitchScene("GameScene");
	}
}

void MenuScene::Render() {
	Update();

	for (Tetromino& tet : tets) {
		tet.Draw();
	}

	// UI rendering
	titleText->Draw();
	startGameBtn->Draw();
	exitGameBtn->Draw();
}

MenuScene::~MenuScene() {
	UnloadFont(font);

	delete titleText;
	delete startGameBtn;
	delete exitGameBtn;
}