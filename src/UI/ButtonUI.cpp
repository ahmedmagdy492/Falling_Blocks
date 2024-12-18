extern "C" {
#include <raylib.h>
}

#include "../../include/UI/ButtonUI.h"

#include <iostream>

ButtonUI::ButtonUI(const char* text, Font font, int fontSize, Vector2 position, void (*onClick)()) : text(text), font(font), position(position), onClick(onClick), padding({ 10, 10, 10, 10}), fontSize(fontSize) {
	Vector2 textSize = MeasureTextEx(font, text, fontSize, 0.0f);
	size = { textSize.x + padding.x + padding.width, textSize.y + padding.y + padding.height };
}

void ButtonUI::Draw() {
	Vector2 textSize = MeasureTextEx(font, text, fontSize, 0.0f);
	DrawTextEx(font, text, { position.x + (size.x - textSize.x) / 2, position.y + (size.y - textSize.y) / 2 }, fontSize, 0.0f, WHITE);

	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos, { position.x, position.y, position.x + size.x, position.y + size.y })) {
		if (onClick) {
			onClick();
		}
	}
}

void ButtonUI::CenterButton(int screenWidth, int screenHeight) {
	this->position = { (screenWidth - size.x) / 2, (screenHeight - size.y) / 2 };
}

void ButtonUI::PlaceBeneath(const ButtonUI& otherButton, float margin) {
	int maxSizeX = std::max(size.x, otherButton.size.x);
	int minSizeX = std::min(size.x, otherButton.size.x);
	this->position = { otherButton.position.x + (maxSizeX - minSizeX) / 2, otherButton.position.y + otherButton.size.y + margin};
}

ButtonUI::~ButtonUI() {

}